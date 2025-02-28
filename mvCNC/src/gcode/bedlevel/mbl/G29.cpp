/**
 * Modern Vintage CNC Firmware
*/

/**
 * G29.cpp - Mesh Bed Leveling
 */

#include "../../../inc/mvCNCConfig.h"

#if ENABLED(MESH_BED_LEVELING)

#include "../../../feature/bedlevel/bedlevel.h"

#include "../../gcode.h"
#include "../../queue.h"

#include "../../../libs/buzzer.h"
#include "../../../lcd/mvcncui.h"
#include "../../../module/motion.h"
#include "../../../module/stepper.h"

#if ENABLED(EXTENSIBLE_UI)
  #include "../../../lcd/extui/ui_api.h"
#elif ENABLED(DWIN_CREALITY_LCD_ENHANCED)
  #include "../../../lcd/e3v2/proui/dwin.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_LEVELING_FEATURE)
#include "../../../core/debug_out.h"

// Save 130 bytes with non-duplication of PSTR
inline void echo_not_entered(const char c) { SERIAL_CHAR(c); SERIAL_ECHOLNPGM(" not entered."); }

/**
 * G29: Mesh-based Z probe, probes a grid and produces a
 *      mesh to compensate for variable bed height
 *
 * Parameters With MESH_BED_LEVELING:
 *
 *  S0              Report the current mesh values
 *  S1              Start probing mesh points
 *  S2              Probe the next mesh point
 *  S3 In Jn Zn.nn  Manually modify a single point
 *  S4 Zn.nn        Set z offset. Positive away from bed, negative closer to bed.
 *  S5              Reset and disable mesh
 */
void GcodeSuite::G29() {
  DEBUG_SECTION(log_G29, "G29", true);

  // G29 Q is also available if debugging
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    const bool seenQ = parser.seen_test('Q');
    if (seenQ || DEBUGGING(LEVELING)) {
      log_machine_info();
      if (seenQ) return;
    }
  #endif

  TERN_(REPORT_STATUS_TO_HOST, set_and_report_grblstate(M_PROBE));

  static int mbl_probe_index = -1;

  MeshLevelingState state = (MeshLevelingState)parser.byteval('S', (int8_t)MeshReport);
  if (!WITHIN(state, 0, 5)) {
    SERIAL_ECHOLNPGM("S out of range (0-5).");
    return;
  }

  int8_t ix, iy;
  ix = iy = 0;

  switch (state) {
    case MeshReport:
      SERIAL_ECHOPGM("Mesh Bed Leveling ");
      if (leveling_is_valid()) {
        serialprintln_onoff(planner.leveling_active);
        mbl.report_mesh();
      }
      else
        SERIAL_ECHOLNPGM("has no data.");
      break;

    case MeshStart:
      mbl.reset();
      mbl_probe_index = 0;
      if (!ui.wait_for_move) {
        queue.inject(parser.seen_test('N') ? F("G28" TERN(CAN_SET_LEVELING_AFTER_G28, "L0", "") "\nG29S2") : F("G29S2"));
        TERN_(EXTENSIBLE_UI, ExtUI::onMeshLevelingStart());
        return;
      }
      state = MeshNext;

    case MeshNext:
      if (mbl_probe_index < 0) {
        SERIAL_ECHOLNPGM("Start mesh probing with \"G29 S1\" first.");
        return;
      }
      // For each G29 S2...
      if (mbl_probe_index == 0) {
        // Move close to the bed before the first point
        do_blocking_move_to_z(0.4f
          #ifdef MANUAL_PROBE_START_Z
            + (MANUAL_PROBE_START_Z) - 0.4f
          #endif
        );
      }
      else {
        // Save Z for the previous mesh position
        mbl.set_zigzag_z(mbl_probe_index - 1, current_position.z);
        TERN_(EXTENSIBLE_UI, ExtUI::onMeshUpdate(ix, iy, current_position.z));
        SET_SOFT_ENDSTOP_LOOSE(false);
      }
      // If there's another point to sample, move there with optional lift.
      if (mbl_probe_index < (GRID_MAX_POINTS)) {
        // Disable software endstops to allow manual adjustment
        // If G29 is left hanging without completion they won't be re-enabled!
        SET_SOFT_ENDSTOP_LOOSE(true);
        mbl.zigzag(mbl_probe_index++, ix, iy);
        _manual_goto_xy({ mbl.index_to_xpos[ix], mbl.index_to_ypos[iy] });
      }
      else {
        // Move to the after probing position
        current_position.z = (
          #ifdef Z_AFTER_PROBING
            Z_AFTER_PROBING
          #else
            Z_CLEARANCE_BETWEEN_MANUAL_PROBES
          #endif
        );
        line_to_current_position();
        planner.synchronize();

        // After recording the last point, activate home and activate
        mbl_probe_index = -1;
        SERIAL_ECHOLNPGM("Mesh probing done.");
        TERN_(HAS_STATUS_MESSAGE, LCD_MESSAGE(MSG_MESH_DONE));
        BUZZ(100, 659);
        BUZZ(100, 698);

        home_all_axes();
        set_bed_leveling_enabled(true);

        #if ENABLED(MESH_G28_REST_ORIGIN)
          current_position.z = 0;
          line_to_current_position(homing_feedrate(Z_AXIS));
          planner.synchronize();
        #endif

        TERN_(LCD_BED_LEVELING, ui.wait_for_move = false);
      }
      break;

    case MeshSet:
      if (parser.seenval('I')) {
        ix = parser.value_int();
        if (!WITHIN(ix, 0, (GRID_MAX_POINTS_X) - 1)) {
          SERIAL_ECHOLNPGM("I out of range (0-", (GRID_MAX_POINTS_X) - 1, ")");
          return;
        }
      }
      else
        return echo_not_entered('J');

      if (parser.seenval('J')) {
        iy = parser.value_int();
        if (!WITHIN(iy, 0, (GRID_MAX_POINTS_Y) - 1)) {
          SERIAL_ECHOLNPGM("J out of range (0-", (GRID_MAX_POINTS_Y) - 1, ")");
          return;
        }
      }
      else
        return echo_not_entered('J');

      if (parser.seenval('Z')) {
        mbl.z_values[ix][iy] = parser.value_linear_units();
        TERN_(EXTENSIBLE_UI, ExtUI::onMeshUpdate(ix, iy, mbl.z_values[ix][iy]));
        TERN_(DWIN_CREALITY_LCD_ENHANCED, DWIN_MeshUpdate(ix, iy, mbl.z_values[ix][iy]));
      }
      else
        return echo_not_entered('Z');
      break;

    case MeshSetZOffset:
      if (parser.seenval('Z'))
        mbl.z_offset = parser.value_linear_units();
      else
        return echo_not_entered('Z');
      break;

    case MeshReset:
      reset_bed_level();
      break;

  } // switch(state)

  if (state == MeshNext) {
    SERIAL_ECHOLNPGM("MBL G29 point ", _MIN(mbl_probe_index, GRID_MAX_POINTS), " of ", GRID_MAX_POINTS);
    if (mbl_probe_index > 0) TERN_(HAS_STATUS_MESSAGE, ui.status_printf(0, F(S_FMT " %i/%i"), GET_TEXT(MSG_PROBING_POINT), _MIN(mbl_probe_index, GRID_MAX_POINTS), int(GRID_MAX_POINTS)));
  }

  report_current_position();

  TERN_(REPORT_STATUS_TO_HOST, set_and_report_grblstate(M_IDLE));
}

#endif // MESH_BED_LEVELING
