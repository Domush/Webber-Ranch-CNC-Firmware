/**
 * Modern Vintage CNC Firmware
*/

#include "../../inc/mvCNCConfig.h"

#if HAS_PID_HEATING

#include "../gcode.h"
#include "../../lcd/mvcncui.h"
  #include "../../module/pwm_temp_io.h"

  #if ENABLED(EXTENSIBLE_UI)
    #include "../../lcd/extui/ui_api.h"
  #elif ENABLED(DWIN_CREALITY_LCD_ENHANCED)
    #include "../../lcd/e3v2/proui/dwin.h"
  #endif

/**
 * M303: PID relay autotune
 *
 *  S<temperature>  Set the target temperature. (Default: 150C / 70C)
 *  E<extruder>     Extruder number to tune, or -1 for the bed. (Default: E0)
 *  C<cycles>       Number of times to repeat the procedure. (Minimum: 3, Default: 5)
 *  U<bool>         Flag to apply the result to the current PID values
 *
 * With PID_DEBUG, PID_BED_DEBUG, or PID_CHAMBER_DEBUG:
 *  D               Toggle PID debugging and EXIT without further action.
 */

void GcodeSuite::M303() {

  #if ANY(PID_DEBUG, PID_BED_DEBUG, PID_CHAMBER_DEBUG)
    if (parser.seen_test('D')) {
      fanManager.pid_debug_flag ^= true;
      SERIAL_ECHO_START();
      SERIAL_ECHOPGM("PID Debug ");
      serialprintln_onoff(fanManager.pid_debug_flag);
      return;
    }
  #endif

  const heater_id_t hid = (heater_id_t)parser.intval('E');
  celsius_t default_temp;
  switch (hid) {
    #if ENABLED(PIDTEMP)
      case 0 ... HOTENDS - 1: default_temp = PREHEAT_1_TEMP_HOTEND; break;
    #endif
    #if ENABLED(PIDTEMPBED)
      case H_BED: default_temp = PREHEAT_1_TEMP_BED; break;
    #endif
    #if ENABLED(PIDTEMPCHAMBER)
      case H_CHAMBER: default_temp = PREHEAT_1_TEMP_CHAMBER; break;
    #endif
    default:
      SERIAL_ECHOLNPGM(STR_PID_BAD_HEATER_ID);
      TERN_(EXTENSIBLE_UI, ExtUI::onPidTuning(ExtUI::result_t::PID_BAD_EXTRUDER_NUM));
      TERN_(DWIN_CREALITY_LCD_ENHANCED, DWIN_PidTuning(PID_BAD_EXTRUDER_NUM));
      return;
  }

  const bool seenC = parser.seenval('C');
  const int c = seenC ? parser.value_int() : 5;
  const bool seenS = parser.seenval('S');
  const celsius_t temp = seenS ? parser.value_celsius() : default_temp;
  const bool u = parser.boolval('U');

  #if ENABLED(DWIN_CREALITY_LCD_ENHANCED)
    if (seenC) HMI_data.PidCycles = c;
    if (seenS) { if (hid == H_BED) HMI_data.BedPidT = temp; else HMI_data.HotendPidT = temp; }
  #endif

  #if DISABLED(BUSY_WHILE_HEATING)
    KEEPALIVE_STATE(NOT_BUSY);
  #endif

  LCD_MESSAGE(MSG_PID_AUTOTUNE);
  fanManager.PID_autotune(temp, hid, c, u);
  ui.reset_status();
}

#endif // HAS_PID_HEATING
