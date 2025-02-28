/**
 * Modern Vintage CNC Firmware
*/
#pragma once

/**
 * Gen7 v1.1, v1.2, v1.3 pin assignments
 */

 /**
 * Rev B    26 DEC 2016
 *
 * 1) added pointer to a current Arduino IDE extension
 * 2) added support for M3, M4 & M5 spindle control commands
 * 3) added case light pin definition
 */

/**
 * A useable Arduino IDE extension (board manager) can be found at
 * https://github.com/Lauszus/Sanguino
 *
 * This extension has been tested on Arduino 1.6.12 & 1.8.0
 *
 * Here's the JSON path:
 * https://raw.githubusercontent.com/Lauszus/Sanguino/master/package_lauszus_sanguino_index.json
 *
 * When installing select 1.0.2
 *
 * Installation instructions can be found at https://learn.sparkfun.com/pages/CustomBoardsArduino
 * Just use the above JSON URL instead of Sparkfun's JSON.
 *
 * Once installed select the Sanguino board and then select the CPU.
 */

#define ALLOW_MEGA644
#include "env_validate.h"

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Gen7 v1.1 / 1.2"
#endif

#ifndef GEN7_VERSION
  #define GEN7_VERSION                        12  // v1.x
#endif

//
// Limit Switches
//
#define X_MIN_PIN                              7
#define Y_MIN_PIN                              5
#define Z_MIN_PIN                              1
#define Z_MAX_PIN                              0
#define Y_MAX_PIN                              2
#define X_MAX_PIN                              6

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                      0
#endif

//
// Steppers
//
#define X_STEP_PIN                            19
#define X_DIR_PIN                             18
#define X_ENABLE_PIN                          24

#define Y_STEP_PIN                            23
#define Y_DIR_PIN                             22
#define Y_ENABLE_PIN                          24

#define Z_STEP_PIN                            26
#define Z_DIR_PIN                             25
#define Z_ENABLE_PIN                          24

#define E0_STEP_PIN                           28
#define E0_DIR_PIN                            27
#define E0_ENABLE_PIN                         24

//
// Temperature Sensors
//
#define TEMP_0_PIN                             1  // Analog Input
#define TEMP_BED_PIN                           2  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                           4
#define HEATER_BED_PIN                         3

#if !defined(FAN_PIN) && GEN7_VERSION < 13        // Gen7 v1.3 removed the fan pin
  #define FAN_PIN                             31
#endif

//
// Misc. Functions
//
#define PS_ON_PIN                             15

#if GEN7_VERSION < 13
  #define CASE_LIGHT_PIN                      16  // Hardware PWM
#else                                             // Gen7 v1.3 removed the I2C connector & signals so need to get PWM off the PC power supply header
  #define CASE_LIGHT_PIN                      15  // Hardware PWM
#endif

// All these generations of Gen7 supply thermistor power
// via PS_ON, so ignore bad thermistor readings
//#define BOGUS_TEMPERATURE_GRACE_PERIOD     2000

#define DEBUG_PIN                              0

// RS485 pins
#define TX_ENABLE_PIN                         12
#define RX_ENABLE_PIN                         13

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_ENA_PIN                 10  // Pullup or pulldown!
#define SPINDLE_DIR_PIN                       11
#if GEN7_VERSION < 13
  #define SPINDLE_LASER_PWM_PIN               16  // Hardware PWM
#else                                             // Gen7 v1.3 removed the I2C connector & signals so need to get PWM off the PC power supply header
  #define SPINDLE_LASER_PWM_PIN               15  // Hardware PWM
#endif
