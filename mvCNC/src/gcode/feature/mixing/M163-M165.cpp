/**
 * Modern Vintage CNC Firmware
*/

#include "../../../inc/mvCNCConfig.h"

#if ENABLED(MIXING_EXTRUDER)

#include "../../gcode.h"
#include "../../../feature/mixing.h"

/**
 * M163: Set a single mix factor for a mixing extruder
 *       This is called "weight" by some systems.
 *       Must be followed by M164 to normalize and commit them.
 *
 *   S[index]   The channel index to set
 *   P[float]   The mix value
 */
void GcodeSuite::M163() {
  const int mix_index = parser.intval('S');
  if (mix_index < MIXING_STEPPERS)
    mixer.set_collector(mix_index, parser.floatval('P'));
}

/**
 * M164: Normalize and commit the mix.
 *
 *   S[index]   The virtual tool to store
 *              If 'S' is omitted update the active virtual tool.
 */
void GcodeSuite::M164() {
  #if MIXING_VIRTUAL_TOOLS > 1
    const int tool_index = parser.intval('S', -1);
  #else
    constexpr int tool_index = 0;
  #endif
  if (tool_index >= 0) {
    if (tool_index < MIXING_VIRTUAL_TOOLS)
      mixer.normalize(tool_index);
  }
  else
    mixer.normalize();
}

#if ENABLED(DIRECT_MIXING_IN_G1)

  /**
   * M165: Set multiple mix factors for a mixing extruder.
   *       Omitted factors will be set to 0.
   *       The mix is normalized and stored in the current virtual tool.
   *
   *   A[factor] Mix factor for extruder stepper 1
   *   B[factor] Mix factor for extruder stepper 2
   *   C[factor] Mix factor for extruder stepper 3
   *   D[factor] Mix factor for extruder stepper 4
   *   H[factor] Mix factor for extruder stepper 5
   *   I[factor] Mix factor for extruder stepper 6
   */
  void GcodeSuite::M165() {
    // Get mixing parameters from the GCode
    // The total "must" be 1.0 (but it will be normalized)
    // If no mix factors are given, the old mix is preserved
    const char mixing_codes[] = { LIST_N(MIXING_STEPPERS, 'A', 'B', 'C', 'D', 'H', 'I') };
    uint8_t mix_bits = 0;
    MIXER_STEPPER_LOOP(i) {
      if (parser.seenval(mixing_codes[i])) {
        SBI(mix_bits, i);
        mixer.set_collector(i, parser.value_float());
      }
    }
    // If any mixing factors were included, clear the rest
    // If none were included, preserve the last mix
    if (mix_bits) {
      MIXER_STEPPER_LOOP(i)
        if (!TEST(mix_bits, i)) mixer.set_collector(i, 0.0f);
      mixer.normalize();
    }
  }

#endif // DIRECT_MIXING_IN_G1

#endif // MIXING_EXTRUDER
