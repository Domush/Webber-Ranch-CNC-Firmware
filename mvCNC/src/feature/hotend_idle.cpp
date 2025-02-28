/**
 * Modern Vintage CNC Firmware
*/

/**
 * Hotend Idle Timeout
 * Prevent filament in the nozzle from charring and causing a critical jam.
 */

#include "../inc/mvCNCConfig.h"

#if ENABLED(HOTEND_IDLE_TIMEOUT)

#include "hotend_idle.h"
#include "../gcode/gcode.h"

  #include "../module/pwm_temp_io.h"
  #include "../module/motion.h"
  #include "../module/planner.h"
  #include "../lcd/mvcncui.h"

extern HotendIdleProtection hotend_idle;

millis_t HotendIdleProtection::next_protect_ms = 0;

void HotendIdleProtection::check_hotends(const millis_t &ms) {
  bool do_prot = false;
  HOTEND_LOOP() {
    const bool busy = (TERN0(HAS_RESUME_CONTINUE, wait_for_user) || planner.has_blocks_queued());
    if (fanManager.degHotend(e) >= (HOTEND_IDLE_MIN_TRIGGER) && !busy) {
      do_prot = true; break;
    }
  }
  if (bool(next_protect_ms) != do_prot)
    next_protect_ms = do_prot ? ms + hp_interval : 0;
}

void HotendIdleProtection::check_e_motion(const millis_t &ms) {
  static float old_e_position = 0;
  if (old_e_position != current_position.e) {
    old_e_position = current_position.e;          // Track filament motion
    if (next_protect_ms)                          // If some heater is on then...
      next_protect_ms = ms + hp_interval;         // ...delay the timeout till later
  }
}

void HotendIdleProtection::check() {
  const millis_t ms = millis();                   // Shared millis

  check_hotends(ms);                              // Any hotends need protection?
  check_e_motion(ms);                             // Motion will protect them

  // Hot and not moving for too long...
  if (next_protect_ms && ELAPSED(ms, next_protect_ms))
    timed_out();
}

// Lower (but don't raise) hotend / bed temperatures
void HotendIdleProtection::timed_out() {
  next_protect_ms = 0;
  SERIAL_ECHOLNPGM("Hotend Idle Timeout");
  LCD_MESSAGE(MSG_HOTEND_IDLE_TIMEOUT);
  HOTEND_LOOP() {
    if ((HOTEND_IDLE_NOZZLE_TARGET) < fanManager.degTargetHotend(e))
      fanManager.setTargetHotend(HOTEND_IDLE_NOZZLE_TARGET, e);
  }
  #if HAS_HEATED_BED
  if ((HOTEND_IDLE_BED_TARGET) < fanManager.degTargetBed())
    fanManager.setTargetBed(HOTEND_IDLE_BED_TARGET);
  #endif
}

#endif // HOTEND_IDLE_TIMEOUT
