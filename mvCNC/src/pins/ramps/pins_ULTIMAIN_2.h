/**
 * Modern Vintage CNC Firmware
*/
#pragma once

/**
 * Ultiboard v2.0 pin assignments
 */

/**
 * Rev B   2 JAN 2017
 *
 *  Added pin definitions for:
 *    M3, M4 & M5 spindle control commands
 *    case light
 */

#define REQUIRE_MEGA2560
#include "env_validate.h"

#define BOARD_INFO_NAME         "Ultimaker 2.x"
#define DEFAULT_MACHINE_NAME    "Ultimaker"
#define DEFAULT_SOURCE_CODE_URL "github.com/Ultimaker/mvCNC"

//
// Limit Switches
//
#define X_STOP_PIN                            22
#define Y_STOP_PIN                            26
#define Z_STOP_PIN                            29

//
// Steppers
//
#define X_STEP_PIN                            25
#define X_DIR_PIN                             23
#define X_ENABLE_PIN                          27

#define Y_STEP_PIN                            32
#define Y_DIR_PIN                             33
#define Y_ENABLE_PIN                          31

#define Z_STEP_PIN                            35
#define Z_DIR_PIN                             36
#define Z_ENABLE_PIN                          34

#define E0_STEP_PIN                           42
#define E0_DIR_PIN                            43
#define E0_ENABLE_PIN                         37

#define E1_STEP_PIN                           49
#define E1_DIR_PIN                            47
#define E1_ENABLE_PIN                         48

#define MOTOR_CURRENT_PWM_XY_PIN              44
#define MOTOR_CURRENT_PWM_Z_PIN               45
#define MOTOR_CURRENT_PWM_E_PIN               46
// Motor current PWM conversion, PWM value = MotorCurrentSetting * 255 / range
#ifndef MOTOR_CURRENT_PWM_RANGE
  #define MOTOR_CURRENT_PWM_RANGE            2000
#endif
#define DEFAULT_PWM_MOTOR_CURRENT  {1300, 1300, 1250}

//
// Temperature Sensors
//
#define TEMP_0_PIN                             8  // Analog Input
#define TEMP_1_PIN                             9  // Analog Input
#define TEMP_BED_PIN                          10  // Analog Input

//
// Heaters / Fans
//
#define HEATER_0_PIN                           2
#define HEATER_1_PIN                           3
#define HEATER_BED_PIN                         4

#ifndef FAN_PIN
  #define FAN_PIN                              7
#endif

#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                     77
#endif

//
// Misc. Functions
//
#define SDSS                                  53
#define SD_DETECT_PIN                         39
#define LED_PIN                                8
#define SAFETY_TRIGGERED_PIN                  28  // PIN to detect the safety circuit has triggered
#define MAIN_VOLTAGE_MEASURE_PIN              14  // ANALOG PIN to measure the main voltage, with a 100k - 4k7 resitor divider.

//
// LCD / Controller
//
#define BEEPER_PIN                            18

#define LCD_PINS_RS                           20
#define LCD_PINS_ENABLE                       15
#define LCD_PINS_D4                           14
#define LCD_PINS_D5                           21
#define LCD_PINS_D6                            5
#define LCD_PINS_D7                            6

// Buttons are directly attached
#define BTN_EN1                               40
#define BTN_EN2                               41
#define BTN_ENC                               19

//
// M3/M4/M5 - Spindle/Laser Control
//
#if HAS_CUTTER                                    // use the LED_PIN for spindle speed control or case light
  #undef LED_PIN
  #define SPINDLE_DIR_PIN                     16
  #define SPINDLE_LASER_ENA_PIN               17  // Pullup!
  #define SPINDLE_LASER_PWM_PIN                8  // Hardware PWM
#else
  #undef LED_PIN
  #define CASE_LIGHT_PIN                       8
#endif
