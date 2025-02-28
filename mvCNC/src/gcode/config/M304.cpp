/**
 * Modern Vintage CNC Firmware
*/

#include "../../inc/mvCNCConfig.h"

#if ENABLED(PIDTEMPBED)

#include "../gcode.h"
  #include "../../module/pwm_temp_io.h"

/**
 * M304 - Set and/or Report the current Bed PID values
 *
 *  P<pval> - Set the P value
 *  I<ival> - Set the I value
 *  D<dval> - Set the D value
 */
void GcodeSuite::M304() {
  if (!parser.seen("PID")) return M304_report();
  if (parser.seen('P')) fanManager.temp_bed.pid.Kp = parser.value_float();
  if (parser.seen('I')) fanManager.temp_bed.pid.Ki = scalePID_i(parser.value_float());
  if (parser.seen('D')) fanManager.temp_bed.pid.Kd = scalePID_d(parser.value_float());
}

void GcodeSuite::M304_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_BED_PID));
  SERIAL_ECHOLNPGM(
    "  M304 P", fanManager.temp_bed.pid.Kp
    , " I", unscalePID_i(fanManager.temp_bed.pid.Ki)
    , " D", unscalePID_d(fanManager.temp_bed.pid.Kd)
  );
}

#endif // PIDTEMPBED
