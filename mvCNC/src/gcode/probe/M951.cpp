/**
 * Modern Vintage CNC Firmware
*/

#include "../../inc/mvCNCConfigPre.h"

#if ENABLED(MAGNETIC_PARKING_EXTRUDER)

#include "../gcode.h"
#include "../../module/tool_change.h"
#include "../../module/motion.h"

mpe_settings_t mpe_settings;

inline void mpe_settings_report() {
  SERIAL_ECHO_MSG("Magnetic Parking Extruder");
  SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("L: Left parking  :", mpe_settings.parking_xpos[0]);
  SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("R: Right parking :", mpe_settings.parking_xpos[1]);
  SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("I: Grab Offset   :", mpe_settings.grab_distance);
  SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("J: Normal speed  :", long(MMS_TO_MMM(mpe_settings.slow_feedrate)));
  SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("H: High speed    :", long(MMS_TO_MMM(mpe_settings.fast_feedrate)));
  SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("D: Distance trav.:", mpe_settings.travel_distance);
  SERIAL_ECHO_START(); SERIAL_ECHOLNPGM("C: Compenstion   :", mpe_settings.compensation_factor);
}

void mpe_settings_init() {
  constexpr float pex[2] = PARKING_EXTRUDER_PARKING_X;
  mpe_settings.parking_xpos[0]      = pex[0];                         // M951 L
  mpe_settings.parking_xpos[1]      = pex[1];                         // M951 R
  mpe_settings.grab_distance        = PARKING_EXTRUDER_GRAB_DISTANCE; // M951 I
  TERN_(HAS_HOME_OFFSET, set_home_offset(X_AXIS, mpe_settings.grab_distance * -1));
  mpe_settings.slow_feedrate        = MMM_TO_MMS(MPE_SLOW_SPEED);     // M951 J
  mpe_settings.fast_feedrate        = MMM_TO_MMS(MPE_FAST_SPEED);     // M951 H
  mpe_settings.travel_distance      = MPE_TRAVEL_DISTANCE;            // M951 D
  mpe_settings.compensation_factor  = MPE_COMPENSATION;               // M951 C
  mpe_settings_report();
}

void GcodeSuite::M951() {
  if (parser.seenval('L')) mpe_settings.parking_xpos[0] = parser.value_linear_units();
  if (parser.seenval('R')) mpe_settings.parking_xpos[1] = parser.value_linear_units();
  if (parser.seenval('I')) {
    mpe_settings.grab_distance = parser.value_linear_units();
    TERN_(HAS_HOME_OFFSET, set_home_offset(X_AXIS, mpe_settings.grab_distance * -1));
  }
  if (parser.seenval('J')) mpe_settings.slow_feedrate       = MMM_TO_MMS(parser.value_linear_units());
  if (parser.seenval('H')) mpe_settings.fast_feedrate       = MMM_TO_MMS(parser.value_linear_units());
  if (parser.seenval('D')) mpe_settings.travel_distance     = parser.value_linear_units();
  if (parser.seenval('C')) mpe_settings.compensation_factor = parser.value_float();
  if (!parser.seen("CDHIJLR")) mpe_settings_report();
}

#endif // MAGNETIC_PARKING_EXTRUDER
