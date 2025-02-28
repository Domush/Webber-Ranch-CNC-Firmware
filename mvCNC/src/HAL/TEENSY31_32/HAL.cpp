/**
 * Modern Vintage CNC Firmware
*/

/**
 * HAL for Teensy 3.2 (MK20DX256)
 */

#ifdef __MK20DX256__

#include "HAL.h"
#include "../shared/Delay.h"

#include <Wire.h>

#define _IMPLEMENT_SERIAL(X) DefaultSerial##X MSerial##X(false, Serial##X)
#define IMPLEMENT_SERIAL(X)  _IMPLEMENT_SERIAL(X)
#if WITHIN(SERIAL_PORT, 0, 3)
  IMPLEMENT_SERIAL(SERIAL_PORT);
#else
  #error "SERIAL_PORT must be from 0 to 3."
#endif
USBSerialType USBSerial(false, SerialUSB);

uint16_t HAL_adc_result;

static const uint8_t pin2sc1a[] = {
    5, 14, 8, 9, 13, 12, 6, 7, 15, 4, 0, 19, 3, 31, // 0-13, we treat them as A0-A13
    5, 14, 8, 9, 13, 12, 6, 7, 15, 4, // 14-23 (A0-A9)
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, // 24-33
    0+64, 19+64, 3+64, 31+64, // 34-37 (A10-A13)
    26, 22, 23, 27, 29, 30 // 38-43: temp. sensor, VREF_OUT, A14, bandgap, VREFH, VREFL. A14 isn't connected to anything in Teensy 3.0.
};

/*
  // disable interrupts
  void cli() { noInterrupts(); }

  // enable interrupts
  void sei() { interrupts(); }
*/

void HAL_adc_init() {
  analog_init();
  while (ADC0_SC3 & ADC_SC3_CAL) {}; // Wait for calibration to finish
  NVIC_ENABLE_IRQ(IRQ_FTM1);
}

void HAL_clear_reset_source() { }

uint8_t HAL_get_reset_source() {
  switch (RCM_SRS0) {
    case 128: return RST_POWER_ON; break;
    case 64: return RST_EXTERNAL; break;
    case 32: return RST_WATCHDOG; break;
    // case 8: return RST_LOSS_OF_LOCK; break;
    // case 4: return RST_LOSS_OF_CLOCK; break;
    // case 2: return RST_LOW_VOLTAGE; break;
  }
  return 0;
}

void HAL_reboot() { _reboot_Teensyduino_(); }

extern "C" {
  extern char __bss_end;
  extern char __heap_start;
  extern void* __brkval;

  int freeMemory() {
    int free_memory;
    if ((int)__brkval == 0)
      free_memory = ((int)&free_memory) - ((int)&__bss_end);
    else
      free_memory = ((int)&free_memory) - ((int)__brkval);
    return free_memory;
  }
}

void HAL_adc_start_conversion(const uint8_t adc_pin) { ADC0_SC1A = pin2sc1a[adc_pin]; }

uint16_t HAL_adc_get_result() { return ADC0_RA; }

#endif // __MK20DX256__
