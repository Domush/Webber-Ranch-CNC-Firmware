/**
 * Modern Vintage CNC Firmware
*/

/**
 * gcode/temp/M104_M109.cpp
 *
 * Hotend target temperature control
 */

#include "../../inc/mvCNCConfigPre.h"

#if HAS_EXTRUDERS

#include "../gcode.h"
  #include "../../module/pwm_temp_io.h"
  #include "../../module/motion.h"
  #include "../../module/planner.h"
  #include "../../lcd/mvcncui.h"

  #include "../../mvCNCCore.h"  // for startOrResumeJob, etc.

  #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
    #include "../../module/jobcounter.h"
    #if ENABLED(CANCEL_OBJECTS)
      #include "../../feature/cancel_object.h"
    #endif
  #endif

  #if ENABLED(SINGLENOZZLE_STANDBY_TEMP)
    #include "../../module/tool_change.h"
  #endif

/**
 * M104: Set Hotend Temperature target and return immediately
 * M109: Set Hotend Temperature target and wait
 *
 * Parameters
 *  I<preset> : Material Preset index (if material presets are defined)
 *  T<index>  : Tool index. If omitted, applies to the active tool
 *  S<target> : The target temperature in current units. For M109, only wait when heating up.
 *
 * With AUTOTEMP...
 *  F<factor> : Autotemp Scaling Factor. Set non-zero to enable Auto-temp.
 *  S<min>    : Minimum temperature, in current units.
 *  B<max>    : Maximum temperature, in current units.
 *
 * M109 Parameters
 *  R<target> : The target temperature in current units. Wait for heating and cooling.
 *
 * Examples
 *  M104 S100 : Set target to 100° and return.
 *  M109 R150 : Set target to 150°. Wait until the hotend gets close to 150°.
 *
 * With PRINTJOB_TIMER_AUTOSTART turning on heaters will start the CNC job timer
 *  (used by jobIsActive, etc.) and turning off heaters will stop the timer.
 */
void GcodeSuite::M104_M109(const bool isM109) {

  if (DEBUGGING(DRYRUN)) return;

  #if ENABLED(MIXING_EXTRUDER) && MIXING_VIRTUAL_TOOLS > 1
    constexpr int8_t target_extruder = 0;
  #else
    const int8_t target_extruder = get_target_extruder_from_command();
    if (target_extruder < 0) return;
  #endif

  bool got_temp = false;
  celsius_t temp = 0;

  // Accept 'I' if temperature presets are defined
  #if HAS_PREHEAT
    got_temp = parser.seenval('I');
    if (got_temp) {
      const uint8_t index = parser.value_byte();
      temp = ui.material_preset[_MIN(index, PREHEAT_COUNT - 1)].hotend_temp;
    }
  #endif

  // Get the temperature from 'S' or 'R'
  bool no_wait_for_cooling = false;
  if (!got_temp) {
    no_wait_for_cooling = parser.seenval('S');
    got_temp = no_wait_for_cooling || (isM109 && parser.seenval('R'));
    if (got_temp) temp = parser.value_celsius();
  }

  if (got_temp) {
    #if ENABLED(SINGLENOZZLE_STANDBY_TEMP)
    fanManager.singlenozzle_temp[target_extruder] = temp;
    if (target_extruder != active_tool) return;
    #endif
    fanManager.setTargetHotend(temp, target_extruder);

    #if ENABLED(DUAL_X_CARRIAGE)
      if (idex_is_duplicating() && target_extruder == 0)
        fanManager.setTargetHotend(temp ? temp + duplicate_extruder_temp_offset : 0, 1);
    #endif

    #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
      /**
       * Use half EXTRUDE_MINTEMP to allow nozzles to be put into hot
       * standby mode, (e.g., in a dual extruder setup) without affecting
       * the running print timer.
       */
      fanManager.auto_job_check_timer(isM109, true);
    #endif

      if (fanManager.isHeatingHotend(target_extruder) || !no_wait_for_cooling)
        fanManager.set_heating_message(target_extruder);
  }

  TERN_(AUTOTEMP, planner.autotemp_M104_M109());

  if (isM109 && got_temp)
    (void)fanManager.wait_for_hotend(target_extruder, no_wait_for_cooling);
}

#endif // EXTRUDERS
