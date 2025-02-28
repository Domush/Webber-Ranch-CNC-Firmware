/**
 * Modern Vintage CNC Firmware
*/
#pragma once

#include "../inc/mvCNCConfigPre.h"
#include "../module/planner.h"

constexpr uint8_t all_on = 0xFF, all_off = 0x00;

class Backlash {
public:
  #if ENABLED(BACKLASH_GCODE)
    static xyz_float_t distance_mm;
    static uint8_t correction;
    #ifdef BACKLASH_SMOOTHING_MM
      static float smoothing_mm;
    #endif

    static void set_correction(const_float_t v) { correction = _MAX(0, _MIN(1.0, v)) * all_on; }
    static float get_correction() { return float(ui8_to_percent(correction)) / 100.0f; }
  #else
    static constexpr uint8_t correction = (BACKLASH_CORRECTION) * 0xFF;
    static const xyz_float_t distance_mm;
    #ifdef BACKLASH_SMOOTHING_MM
      static constexpr float smoothing_mm = BACKLASH_SMOOTHING_MM;
    #endif
  #endif

  #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
    private:
      static xyz_float_t measured_mm;
      static xyz_uint8_t measured_count;
    public:
      static void measure_with_probe();
  #endif

  static float get_measurement(const AxisEnum a) {
    UNUSED(a);
    // Return the measurement averaged over all readings
    return TERN(MEASURE_BACKLASH_WHEN_PROBING
      , measured_count[a] > 0 ? measured_mm[a] / measured_count[a] : 0
      , 0
    );
  }

  static bool has_measurement(const AxisEnum a) {
    UNUSED(a);
    return TERN0(MEASURE_BACKLASH_WHEN_PROBING, measured_count[a] > 0);
  }

  static bool has_any_measurement() {
    return has_measurement(X_AXIS) || has_measurement(Y_AXIS) || has_measurement(Z_AXIS);
  }

  void add_correction_steps(const int32_t &da, const int32_t &db, const int32_t &dc, const axis_bits_t dm, block_t * const block);
};

extern Backlash backlash;
