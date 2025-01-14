/**
 * Modern Vintage CNC Firmware
*/

/**
 * Support routines for LPC1768
 */

/**
 * Translation of routines & variables used by pinsDebug.h
 */

#define NUMBER_PINS_TOTAL NUM_DIGITAL_PINS
#define pwm_details(pin) pin = pin    // do nothing  // print PWM details
#define pwm_status(pin) false //Print a pin's PWM status. Return true if it's currently a PWM pin.
#define IS_ANALOG(P) (DIGITAL_PIN_TO_ANALOG_PIN(P) >= 0 ? 1 : 0)
#define digitalRead_mod(p) extDigitalRead(p)
#define PRINT_PORT(p)
#define GET_ARRAY_PIN(p) pin_array[p].pin
#define PRINT_ARRAY_NAME(x) do{ sprintf_P(buffer, PSTR("%-" STRINGIFY(MAX_NAME_LENGTH) "s"), pin_array[x].name); SERIAL_ECHO(buffer); }while(0)
#define PRINT_PIN(p) do{ sprintf_P(buffer, PSTR("P%d_%02d"), LPC176x::pin_port(p), LPC176x::pin_bit(p)); SERIAL_ECHO(buffer); }while(0)
#define PRINT_PIN_ANALOG(p) do{ sprintf_P(buffer, PSTR("_A%d     "), LPC176x::pin_get_adc_channel(pin)); SERIAL_ECHO(buffer); }while(0)
#define MULTI_NAME_PAD 17 // space needed to be pretty if not first name assigned to a pin

// pins that will cause hang/reset/disconnect in M43 Toggle and Watch utilities
#ifndef M43_NEVER_TOUCH
  #define M43_NEVER_TOUCH(Q) ((Q) == P0_29 || (Q) == P0_30 || (Q) == P2_09)  // USB pins
#endif

bool GET_PINMODE(const pin_t pin) {
  if (!LPC176x::pin_is_valid(pin) || LPC176x::pin_adc_enabled(pin)) // found an invalid pin or active analog pin
    return false;

  return LPC176x::gpio_direction(pin);
}

#define GET_ARRAY_IS_DIGITAL(x) ((bool) pin_array[x].is_digital)
