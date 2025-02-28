/**
 * Modern Vintage CNC Firmware
*/

//
// Delta Calibrate Menu
//

#include "../../inc/mvCNCConfigPre.h"

#if HAS_MVCNCUI_MENU && EITHER(DELTA_CALIBRATION_MENU, DELTA_AUTO_CALIBRATION)

#include "menu_item.h"
#include "../../module/delta.h"
#include "../../module/motion.h"
#include "../../module/planner.h"

#if HAS_LEVELING
  #include "../../feature/bedlevel/bedlevel.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../extui/ui_api.h"
#endif

void _man_probe_pt(const xy_pos_t &xy) {
  if (!ui.wait_for_move) {
    ui.wait_for_move = true;
    do_blocking_move_to_xy_z(xy, Z_CLEARANCE_BETWEEN_PROBES);
    ui.wait_for_move = false;
    ui.synchronize();
    ui.manual_move.menu_scale = _MAX(PROBE_MANUALLY_STEP, MIN_STEPS_PER_SEGMENT / planner.settings.axis_steps_per_mm[0]); // Use first axis as for delta XYZ should always match
    ui.goto_screen(lcd_move_z);
  }
}

#if ENABLED(DELTA_AUTO_CALIBRATION)

  #if HAS_RESUME_CONTINUE
    #include "../../mvCNCCore.h" // for wait_for_user_response()
  #endif
  #if ENABLED(HOST_PROMPT_SUPPORT)
    #include "../../feature/host_actions.h" // for hostui.prompt_do
  #endif

  float lcd_probe_pt(const xy_pos_t &xy) {
    _man_probe_pt(xy);
    ui.defer_status_screen();
    TERN_(HOST_PROMPT_SUPPORT, hostui.prompt_do(PROMPT_USER_CONTINUE, F("Delta Calibration in progress"), FPSTR(CONTINUE_STR)));
    TERN_(EXTENSIBLE_UI, ExtUI::onUserConfirmRequired(F("Delta Calibration in progress")));
    TERN_(HAS_RESUME_CONTINUE, wait_for_user_response());
    ui.goto_previous_screen_no_defer();
    return current_position.z;
  }

#endif

#if ENABLED(DELTA_CALIBRATION_MENU)

  #include "../../gcode/queue.h"

  void _lcd_calibrate_homing() {
    _lcd_draw_homing();
    if (all_axes_homed()) ui.goto_previous_screen();
  }

  void _lcd_delta_calibrate_home() {
    queue.inject_P(G28_STR);
    ui.goto_screen(_lcd_calibrate_homing);
  }

  void _goto_tower_a(const_float_t a) {
    constexpr float dcr = DELTA_PRINTABLE_RADIUS;
    xy_pos_t tower_vec = { cos(RADIANS(a)), sin(RADIANS(a)) };
    _man_probe_pt(tower_vec * dcr);
  }
  void _goto_tower_x() { _goto_tower_a(210); }
  void _goto_tower_y() { _goto_tower_a(330); }
  void _goto_tower_z() { _goto_tower_a( 90); }
  void _goto_center()  { xy_pos_t ctr{0}; _man_probe_pt(ctr); }

#endif

void lcd_delta_settings() {
  auto _recalc_delta_settings = []{
    TERN_(HAS_LEVELING, reset_bed_level()); // After changing kinematics bed-level data is no longer valid
    recalc_delta_settings();
  };
  START_MENU();
  BACK_ITEM(MSG_DELTA_CALIBRATE);
  EDIT_ITEM(float52sign, MSG_DELTA_HEIGHT, &delta_height, delta_height - 10, delta_height + 10, _recalc_delta_settings);
  #define EDIT_ENDSTOP_ADJ(LABEL,N) EDIT_ITEM_P(float43, PSTR(LABEL), &delta_endstop_adj.N, -5, 0, _recalc_delta_settings)
  EDIT_ENDSTOP_ADJ("Ex", a);
  EDIT_ENDSTOP_ADJ("Ey", b);
  EDIT_ENDSTOP_ADJ("Ez", c);
  EDIT_ITEM(float52sign, MSG_DELTA_RADIUS, &delta_radius, delta_radius - 5, delta_radius + 5, _recalc_delta_settings);
  #define EDIT_ANGLE_TRIM(LABEL,N) EDIT_ITEM_P(float43, PSTR(LABEL), &delta_tower_angle_trim.N, -5, 5, _recalc_delta_settings)
  EDIT_ANGLE_TRIM("Tx", a);
  EDIT_ANGLE_TRIM("Ty", b);
  EDIT_ANGLE_TRIM("Tz", c);
  EDIT_ITEM(float52sign, MSG_DELTA_DIAG_ROD, &delta_diagonal_rod, delta_diagonal_rod - 5, delta_diagonal_rod + 5, _recalc_delta_settings);
  END_MENU();
}

void menu_delta_calibrate() {
  #if ENABLED(DELTA_CALIBRATION_MENU)
    const bool all_homed = all_axes_homed();  // Acquire ahead of loop
  #endif

  START_MENU();
  BACK_ITEM(MSG_MAIN);

  #if ENABLED(DELTA_AUTO_CALIBRATION)
    GCODES_ITEM(MSG_DELTA_AUTO_CALIBRATE, PSTR("G33"));
    #if ENABLED(EEPROM_SETTINGS)
      ACTION_ITEM(MSG_STORE_EEPROM, ui.store_settings);
      ACTION_ITEM(MSG_LOAD_EEPROM, ui.load_settings);
    #endif
  #endif

  SUBMENU(MSG_DELTA_SETTINGS, lcd_delta_settings);

  #if ENABLED(DELTA_CALIBRATION_MENU)
    SUBMENU(MSG_AUTO_HOME, _lcd_delta_calibrate_home);
    if (all_homed) {
      SUBMENU(MSG_DELTA_CALIBRATE_X, _goto_tower_x);
      SUBMENU(MSG_DELTA_CALIBRATE_Y, _goto_tower_y);
      SUBMENU(MSG_DELTA_CALIBRATE_Z, _goto_tower_z);
      SUBMENU(MSG_DELTA_CALIBRATE_CENTER, _goto_center);
    }
  #endif

  END_MENU();
}

#endif // HAS_MVCNCUI_MENU && (DELTA_CALIBRATION_MENU || DELTA_AUTO_CALIBRATION)
