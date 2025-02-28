/**
 * Modern Vintage CNC Firmware
*/

#include "../gcode.h"
#include "../../module/motion.h"

#include "../../mvCNCCore.h"

#if BOTH(FWRETRACT, FWRETRACT_AUTORETRACT)
  #include "../../feature/fwretract.h"
#endif

#include "../../sd/cardreader.h"

#if ENABLED(NANODLP_Z_SYNC)
  #include "../../module/stepper.h"
#endif

extern xyze_pos_t destination;

#if ENABLED(VARIABLE_G0_FEEDRATE)
  feedRate_t fast_move_feedrate = MMM_TO_MMS(G0_FEEDRATE);
#endif

/**
 * G0, G1: Coordinated movement of X Y Z E axes
 */
void GcodeSuite::G0_G1(TERN_(HAS_FAST_MOVES, const bool fast_move/*=false*/)) {

  if (IsRunning()
    #if ENABLED(NO_MOTION_BEFORE_HOMING)
      && !homing_needed_error(
        LINEAR_AXIS_GANG(
            (parser.seen_test('X') ? _BV(X_AXIS) : 0),
          | (parser.seen_test('Y') ? _BV(Y_AXIS) : 0),
          | (parser.seen_test('Z') ? _BV(Z_AXIS) : 0),
          | (parser.seen_test(AXIS4_NAME) ? _BV(I_AXIS) : 0),
          | (parser.seen_test(AXIS5_NAME) ? _BV(J_AXIS) : 0),
          | (parser.seen_test(AXIS6_NAME) ? _BV(K_AXIS) : 0))
      )
    #endif
  ) {
    TERN_(REPORT_STATUS_TO_HOST, set_and_report_grblstate(M_RUNNING));

    #ifdef G0_FEEDRATE
      feedRate_t old_feedrate;
      #if ENABLED(VARIABLE_G0_FEEDRATE)
        if (fast_move) {
          old_feedrate = feedrate_mm_s;             // Back up the (old) motion mode feedrate
          feedrate_mm_s = fast_move_feedrate;       // Get G0 feedrate from last usage
        }
      #endif
    #endif

    get_destination_from_command();                 // Get X Y [Z[I[J[K]]]] [E] F (and set cutter power)

    #ifdef G0_FEEDRATE
      if (fast_move) {
        #if ENABLED(VARIABLE_G0_FEEDRATE)
          fast_move_feedrate = feedrate_mm_s;       // Save feedrate for the next G0
        #else
          old_feedrate = feedrate_mm_s;             // Back up the (new) motion mode feedrate
          feedrate_mm_s = MMM_TO_MMS(G0_FEEDRATE);  // Get the fixed G0 feedrate
        #endif
      }
    #endif

    #if BOTH(FWRETRACT, FWRETRACT_AUTORETRACT)

      if (MIN_AUTORETRACT <= MAX_AUTORETRACT) {
        // When M209 Autoretract is enabled, convert E-only moves to firmware retract/recover moves
        if (fwretract.autoretract_enabled && parser.seen_test('E')
          && !parser.seen(LINEAR_AXIS_GANG("X", "Y", "Z", STR_I, STR_J, STR_K))
        ) {
          const float echange = destination.e - current_position.e;
          // Is this a retract or recover move?
          if (WITHIN(ABS(echange), MIN_AUTORETRACT, MAX_AUTORETRACT) && fwretract.retracted[active_tool] == (echange > 0.0)) {
            current_position.e = destination.e;       // Hide a G1-based retract/recover from calculations
            sync_plan_position_e();                   // AND from the planner
            return fwretract.retract(echange < 0.0);  // Firmware-based retract/recover (double-retract ignored)
          }
        }
      }

    #endif // FWRETRACT

    #if IS_SCARA
      fast_move ? prepare_fast_move_to_destination() : prepare_line_to_destination();
    #else
      prepare_line_to_destination();
    #endif

    #ifdef G0_FEEDRATE
      // Restore the motion mode feedrate
      if (fast_move) feedrate_mm_s = old_feedrate;
    #endif

    #if ENABLED(NANODLP_Z_SYNC)
      #if ENABLED(NANODLP_ALL_AXIS)
        #define _MOVE_SYNC parser.seenval('X') || parser.seenval('Y') || parser.seenval('Z')  // For any move wait and output sync message
      #else
        #define _MOVE_SYNC parser.seenval('Z')  // Only for Z move
      #endif
      if (_MOVE_SYNC) {
        planner.synchronize();
        SERIAL_ECHOLNPGM(STR_Z_MOVE_COMP);
      }
      TERN_(REPORT_STATUS_TO_HOST, set_and_report_grblstate(M_IDLE));
    #else
      TERN_(REPORT_STATUS_DURING_MOVES, report_current_grblstate_moving());
    #endif
  }
  #ifdef G0_FEEDRATE
    if (fast_move) TERN_(M114_RAPID_REPORTING, report_current_position());
  #endif
}
