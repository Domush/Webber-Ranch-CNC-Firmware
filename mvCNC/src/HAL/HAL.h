/**
 * Modern Vintage CNC Firmware
*/
#pragma once

#include "platforms.h"

#ifndef GCC_VERSION
  #define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

#include HAL_PATH(.,HAL.h)

#define HAL_ADC_RANGE _BV(HAL_ADC_RESOLUTION)

#ifndef I2C_ADDRESS
  #define I2C_ADDRESS(A) uint8_t(A)
#endif

// Needed for AVR sprintf_P PROGMEM extension
#ifndef S_FMT
  #define S_FMT "%s"
#endif

// String helper
#ifndef PGMSTR
  #define PGMSTR(NAM,STR) const char NAM[] = STR
#endif

inline void watchdog_refresh() {
  TERN_(USE_WATCHDOG, HAL_watchdog_refresh());
}
