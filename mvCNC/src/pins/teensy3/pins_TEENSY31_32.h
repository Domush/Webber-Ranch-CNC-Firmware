/**
 * Modern Vintage CNC Firmware
*/
#pragma once

/**
 * Teensy 3.1 (MK20DX256) and Teensy 3.2 (MK20DX256) Breadboard pin assignments
 * Requires the Teensyduino software with Teensy 3.1 or Teensy 3.2 selected in Arduino IDE!
 * https://www.pjrc.com/teensy/teensyduino.html
 */

#if NOT_TARGET(IS_TEENSY_31_32)
  #error "Oops! Select 'Teensy 3.1' or 'Teensy 3.2' in 'Tools > Board.'"
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Teensy3.2"
#endif

//
// Limit Switches
//
#define X_STOP_PIN                             3
#define Y_STOP_PIN                             4
#define Z_STOP_PIN                             5

//
// Steppers
//
#define X_STEP_PIN                             5
#define X_DIR_PIN                              6
#define X_ENABLE_PIN                           2

#define Y_STEP_PIN                             7
#define Y_DIR_PIN                              8
#define Y_ENABLE_PIN                           2

#define Z_STEP_PIN                             9
#define Z_DIR_PIN                             10
#define Z_ENABLE_PIN                           2

#define E0_STEP_PIN                           11
#define E0_DIR_PIN                            12
#define E0_ENABLE_PIN                          2

//#define E1_STEP_PIN                         33
//#define E1_DIR_PIN                          34
//#define E1_ENABLE_PIN                       35

//
// Heaters / Fans
//
#define HEATER_0_PIN                          20
//#define HEATER_1_PIN                        36
#define HEATER_BED_PIN                        21
#ifndef FAN_PIN
  #define FAN_PIN                             22
#endif

//
// Temperature Sensors
//
#define TEMP_0_PIN                            14  // Analog Input - Extruder 2 => A2
//#define TEMP_1_PIN                           0  // Analog Input
#define TEMP_BED_PIN                          15  // Analog Input - Bed

#ifndef FILWIDTH_PIN
  #define FILWIDTH_PIN                         6  // Analog Input
#endif

//
// Misc. Functions
//
#define LED_PIN                               13
//#define SOL1_PIN                            28
//#define SDSS                                16  // 8

//
// LCD / Controller
//

/*
#if HAS_WIRED_LCD
  #define LCD_PINS_RS                         40
  #define LCD_PINS_ENABLE                     41
  #define LCD_PINS_D4                         42
  #define LCD_PINS_D5                         43
  #define LCD_PINS_D6                         44
  #define LCD_PINS_D7                         45
  #define BTN_EN1                             46
  #define BTN_EN2                             47
  #define BTN_ENC                             48
#endif
*/
