/**
 * Modern Vintage CNC Firmware
*/

#include "utility.h"

#include "../mvCNCCore.h"
#include "../module/pwm_temp_io.h"

void safe_delay(millis_t ms) {
  while (ms > 50) {
    ms -= 50;
    delay(50);
    fanManager.manage_heater();
  }
  delay(ms);
  fanManager.manage_heater(); // This keeps us safe if too many small safe_delay() calls are made
}

// A delay to provide brittle hosts time to receive bytes
#if ENABLED(SERIAL_OVERRUN_PROTECTION)

  #include "../gcode/gcode.h" // for set_autoreport_paused

  void serial_delay(const millis_t ms) {
    const bool was = gcode.set_autoreport_paused(true);
    safe_delay(ms);
    gcode.set_autoreport_paused(was);
  }
#endif

#if ENABLED(DEBUG_LEVELING_FEATURE)

  #include "../module/probe.h"
  #include "../module/motion.h"
  #include "../module/stepper.h"
  #include "../libs/numtostr.h"
  #include "../feature/bedlevel/bedlevel.h"

  void log_machine_info() {
    SERIAL_ECHOLNPGM("Machine Type: "
      TERN_(DELTA,         "Delta")
      TERN_(IS_SCARA,      "SCARA")
      TERN_(IS_CORE,       "Core")
      TERN_(MARKFORGED_XY, "MarkForgedXY")
      TERN_(MARKFORGED_YX, "MarkForgedYX")
      TERN_(IS_CARTESIAN,  "Cartesian")
    );

    SERIAL_ECHOLNPGM("Probe: "
      TERN_(PROBE_MANUALLY, "PROBE_MANUALLY")
      TERN_(NOZZLE_AS_PROBE, "NOZZLE_AS_PROBE")
      TERN_(FIX_MOUNTED_PROBE, "FIX_MOUNTED_PROBE")
      TERN_(HAS_Z_SERVO_PROBE, TERN(BLTOUCH, "BLTOUCH", "SERVO PROBE"))
      TERN_(TOUCH_MI_PROBE, "TOUCH_MI_PROBE")
      TERN_(Z_PROBE_SLED, "Z_PROBE_SLED")
      TERN_(Z_PROBE_ALLEN_KEY, "Z_PROBE_ALLEN_KEY")
      TERN_(SOLENOID_PROBE, "SOLENOID_PROBE")
      TERN_(MAGLEV4, "MAGLEV4")
      IF_DISABLED(PROBE_SELECTED, "NONE")
    );

    #if HAS_BED_PROBE

      #if !HAS_PROBE_XY_OFFSET
        SERIAL_ECHOPGM("Probe Offset X0 Y0 Z", probe.offset.z, " (");
      #else
        SERIAL_ECHOPGM_P(PSTR("Probe Offset X"), probe.offset_xy.x, SP_Y_STR, probe.offset_xy.y, SP_Z_STR, probe.offset.z);
        if (probe.offset_xy.x > 0)
          SERIAL_ECHOPGM(" (Right");
        else if (probe.offset_xy.x < 0)
          SERIAL_ECHOPGM(" (Left");
        else if (probe.offset_xy.y != 0)
          SERIAL_ECHOPGM(" (Middle");
        else
          SERIAL_ECHOPGM(" (Aligned With");

        if (probe.offset_xy.y > 0)
          SERIAL_ECHOF(F(TERN(IS_SCARA, "-Distal", "-Back")));
        else if (probe.offset_xy.y < 0)
          SERIAL_ECHOF(F(TERN(IS_SCARA, "-Proximal", "-Front")));
        else if (probe.offset_xy.x != 0)
          SERIAL_ECHOPGM("-Center");

        SERIAL_ECHOPGM(" & ");

      #endif

      SERIAL_ECHOF(probe.offset.z < 0 ? F("Below") : probe.offset.z > 0 ? F("Above") : F("Same Z as"));
      SERIAL_ECHOLNPGM(" Nozzle)");

    #endif

    #if HAS_ABL_OR_UBL
      SERIAL_ECHOPGM("Auto Bed Leveling: "
        TERN_(AUTO_BED_LEVELING_LINEAR, "LINEAR")
        TERN_(AUTO_BED_LEVELING_BILINEAR, "BILINEAR")
        TERN_(AUTO_BED_LEVELING_3POINT, "3POINT")
        TERN_(AUTO_BED_LEVELING_UBL, "UBL")
      );

      if (planner.leveling_active) {
        SERIAL_ECHOLNPGM(" (enabled)");
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          if (planner.z_fade_height)
            SERIAL_ECHOLNPGM("Z Fade: ", planner.z_fade_height);
        #endif
        #if ABL_PLANAR
          SERIAL_ECHOPGM("ABL Adjustment");
          LOOP_LINEAR_AXES(a) {
            const float v = planner.get_axis_position_mm(AxisEnum(a)) - current_position[a];
            SERIAL_CHAR(' ', AXIS_CHAR(a));
            if (v > 0) SERIAL_CHAR('+');
            SERIAL_DECIMAL(v);
          }
        #else
          #if ENABLED(AUTO_BED_LEVELING_UBL)
            SERIAL_ECHOPGM("UBL Adjustment Z");
            const float rz = ubl.get_z_correction(current_position);
          #elif ENABLED(AUTO_BED_LEVELING_BILINEAR)
            SERIAL_ECHOPGM("ABL Adjustment Z");
            const float rz = bilinear_z_offset(current_position);
          #endif
          SERIAL_ECHO(ftostr43sign(rz, '+'));
          #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
            if (planner.z_fade_height) {
              SERIAL_ECHOPGM(" (", ftostr43sign(rz * planner.fade_scaling_factor_for_z(current_position.z), '+'));
              SERIAL_CHAR(')');
            }
          #endif
        #endif
      }
      else
        SERIAL_ECHOLNPGM(" (disabled)");

      SERIAL_EOL();

    #elif ENABLED(MESH_BED_LEVELING)

      SERIAL_ECHOPGM("Mesh Bed Leveling");
      if (planner.leveling_active) {
        SERIAL_ECHOLNPGM(" (enabled)");
        SERIAL_ECHOPGM("MBL Adjustment Z", ftostr43sign(mbl.get_z(current_position), '+'));
        #if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
          if (planner.z_fade_height) {
            SERIAL_ECHOPGM(" (", ftostr43sign(
              mbl.get_z(current_position, planner.fade_scaling_factor_for_z(current_position.z)), '+'
            ));
            SERIAL_CHAR(')');
          }
        #endif
      }
      else
        SERIAL_ECHOPGM(" (disabled)");

      SERIAL_EOL();

    #endif // MESH_BED_LEVELING
  }

#endif // DEBUG_LEVELING_FEATURE
