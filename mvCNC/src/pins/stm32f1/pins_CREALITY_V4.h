/**
 * Modern Vintage CNC Firmware
*/
#pragma once

/**
 * Creality 4.2.x (STM32F103RE / STM32F103RC) board pin assignments
 */

#include "env_validate.h"

#if HAS_MULTI_HOTEND || E_STEPPERS > 1
  #error "Creality V4 only supports one hotend / E-stepper. Comment out this line to continue."
#endif

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME      "Creality V4"
#endif
#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "Ender 3 V2"
#endif

#define BOARD_NO_NATIVE_USB

//
// EEPROM
//
#if NO_EEPROM_SELECTED
  #define IIC_BL24CXX_EEPROM                      // EEPROM on I2C-0
  //#define SDCARD_EEPROM_EMULATION
#endif

#if ENABLED(IIC_BL24CXX_EEPROM)
  #define IIC_EEPROM_SDA                    PA11
  #define IIC_EEPROM_SCL                    PA12
#define MVCNC_EEPROM_SIZE               0x800  // 2Kb (24C16)
#elif ENABLED(SDCARD_EEPROM_EMULATION)
#define MVCNC_EEPROM_SIZE               0x800  // 2Kb
#endif

//
// Servos
//
#ifndef SERVO0_PIN
  #ifndef HAS_PIN_27_BOARD
    #define SERVO0_PIN                      PB0   // BLTouch OUT
  #else
    #define SERVO0_PIN                      PC6
  #endif
#endif

//
// Limit Switches
//
#define X_STOP_PIN                          PA5
#define Y_STOP_PIN                          PA6
#ifndef Z_STOP_PIN
  #define Z_STOP_PIN                        PA7
#endif

#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                   PB1   // BLTouch IN
#endif

//
// Filament Runout Sensor
//
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN                    PA4   // "Pulled-high"
#endif

//
// Steppers
//
#ifndef X_STEP_PIN
  #define X_STEP_PIN                        PC2
#endif
#ifndef X_DIR_PIN
  #define X_DIR_PIN                         PB9
#endif
#define X_ENABLE_PIN                        PC3   // Shared

#ifndef Y_STEP_PIN
  #define Y_STEP_PIN                        PB8
#endif
#ifndef Y_DIR_PIN
  #define Y_DIR_PIN                         PB7
#endif
#define Y_ENABLE_PIN                X_ENABLE_PIN

#ifndef Z_STEP_PIN
  #define Z_STEP_PIN                        PB6
#endif
#ifndef Z_DIR_PIN
  #define Z_DIR_PIN                         PB5
#endif
#define Z_ENABLE_PIN                X_ENABLE_PIN

#ifndef E0_STEP_PIN
  #define E0_STEP_PIN                       PB4
#endif
#ifndef E0_DIR_PIN
  #define E0_DIR_PIN                        PB3
#endif
#define E0_ENABLE_PIN               X_ENABLE_PIN

//
// Release PB4 (Y_ENABLE_PIN) from JTAG NRST role
//
#define DISABLE_DEBUG

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PC5   // TH1
#define TEMP_BED_PIN                        PC4   // TB1

//
// Heaters / Fans
//
#ifndef HEATER_0_PIN
  #define HEATER_0_PIN                      PA1   // HEATER1
#endif
#ifndef HEATER_BED_PIN
  #define HEATER_BED_PIN                    PA2   // HOT BED
#endif
#ifndef FAN_PIN
  #define FAN_PIN                           PA0   // FAN
#endif
#define FAN_SOFT_PWM_REQUIRED

//
// SD Card
//
#define SD_DETECT_PIN                       PC7
#define SDCARD_CONNECTION                ONBOARD
#define SDIO_SUPPORT
#define NO_SD_HOST_DRIVE                          // This board's SD is only seen by the cnc

#if ENABLED(CR10_STOCKDISPLAY)

  #if ENABLED(RET6_12864_LCD)

    // RET6 12864 LCD
    #define LCD_PINS_RS                     PB12
    #define LCD_PINS_ENABLE                 PB15
    #define LCD_PINS_D4                     PB13

    #define BTN_ENC                         PB2
    #define BTN_EN1                         PB10
    #define BTN_EN2                         PB14

    #ifndef HAS_PIN_27_BOARD
      #define BEEPER_PIN                    PC6
    #endif

  #elif ENABLED(VET6_12864_LCD)

    // VET6 12864 LCD
    #define LCD_PINS_RS                     PA4
    #define LCD_PINS_ENABLE                 PA7
    #define LCD_PINS_D4                     PA5

    #define BTN_ENC                         PC5
    #define BTN_EN1                         PB10
    #define BTN_EN2                         PA6

  #else
    #error "Define RET6_12864_LCD or VET6_12864_LCD to select pins for CR10_STOCKDISPLAY with the Creality V4 controller."
  #endif

#elif HAS_DWIN_E3V2 || IS_DWIN_MVCNCUI

  // RET6 DWIN ENCODER LCD
  #define BTN_ENC                           PB14
  #define BTN_EN1                           PB15
  #define BTN_EN2                           PB12

  //#define LCD_LED_PIN                     PB2
  #ifndef BEEPER_PIN
    #define BEEPER_PIN                      PB13
  #endif

#elif ENABLED(DWIN_VET6_CREALITY_LCD)

  // VET6 DWIN ENCODER LCD
  #define BTN_ENC                           PA6
  #define BTN_EN1                           PA7
  #define BTN_EN2                           PA4

  #define BEEPER_PIN                        PA5

#endif
