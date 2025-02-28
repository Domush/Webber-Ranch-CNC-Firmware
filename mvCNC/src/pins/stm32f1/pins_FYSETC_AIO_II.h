/**
 * Modern Vintage CNC Firmware
*/
#pragma once

#include "env_validate.h"

#define BOARD_INFO_NAME   "FYSETC AIO II"
#define BOARD_WEBSITE_URL "fysetc.com"

#define BOARD_NO_NATIVE_USB

#define DISABLE_JTAG

#define pins_v2_20190128                          // new pins define

// Ignore temp readings during development.
//#define BOGUS_TEMPERATURE_GRACE_PERIOD    2000

//
// Flash EEPROM Emulation
//
#if EITHER(NO_EEPROM_SELECTED, FLASH_EEPROM_EMULATION)
  #define FLASH_EEPROM_EMULATION
  #define EEPROM_PAGE_SIZE     (0x800U)           // 2KB
  #define EEPROM_START_ADDRESS (0x8000000UL + (STM32_FLASH_SIZE) * 1024UL - (EEPROM_PAGE_SIZE) * 2UL)
#define MVCNC_EEPROM_SIZE    EEPROM_PAGE_SIZE  // 2KB
#endif

//
// Limit Switches
//
#define X_STOP_PIN                          PA1
#define Y_STOP_PIN                          PA0
#define Z_STOP_PIN                          PB14

//
// Filament runout
//
#ifdef pins_v2_20190128
  #define FIL_RUNOUT_PIN                    PB15
#else
  #define FIL_RUNOUT_PIN                    PB5
#endif

//
// Steppers
//
#define X_STEP_PIN                          PB8
#define X_DIR_PIN                           PB9
#define X_ENABLE_PIN                        PA8

#define Y_STEP_PIN                          PB2
#ifdef pins_v2_20190128
  #define Y_DIR_PIN                         PB3
#else
  #define Y_DIR_PIN                         PB0
#endif
#define Y_ENABLE_PIN                        PB1

#define Z_STEP_PIN                          PC0
#define Z_DIR_PIN                           PC1
#define Z_ENABLE_PIN                        PC2

#define E0_STEP_PIN                         PC15
#define E0_DIR_PIN                          PC14
#define E0_ENABLE_PIN                       PC13

#if HAS_TMC_UART
  /**
   * TMC2208/TMC2209 stepper drivers
   */

  // Hardware serial with switch
  #define X_HARDWARE_SERIAL  MSerial2
  #define Y_HARDWARE_SERIAL  MSerial2
  #define Z_HARDWARE_SERIAL  MSerial2
  #define E0_HARDWARE_SERIAL MSerial2

  // Default TMC slave addresses
  #ifndef X_SLAVE_ADDRESS
    #define X_SLAVE_ADDRESS  0
  #endif
  #ifndef Y_SLAVE_ADDRESS
    #define Y_SLAVE_ADDRESS  1
  #endif
  #ifndef Z_SLAVE_ADDRESS
    #define Z_SLAVE_ADDRESS  2
  #endif
  #ifndef E0_SLAVE_ADDRESS
    #define E0_SLAVE_ADDRESS 3
  #endif

  // The 4xTMC2209 module doesn't have a serial multiplexer and
  // needs to set *_SLAVE_ADDRESS in Configuration_adv.h for X,Y,Z,E0
  #if HAS_DRIVER(TMC2208)
    #define TMC_SERIAL_MULTIPLEXER
    #define SERIAL_MUL_PIN1                 PB13
    #define SERIAL_MUL_PIN2                 PB12
  #endif

  // Reduce baud rate to improve software serial reliability
  #define TMC_BAUD_RATE                    19200
#endif

//
// Stepper current PWM
//
#ifndef MOTOR_CURRENT_PWM_RANGE
  #define MOTOR_CURRENT_PWM_RANGE           1500  // origin:2000
#endif
#define DEFAULT_PWM_MOTOR_CURRENT { 500, 500, 400 } // origin: {1300,1300,1250}

//
// Heaters / Fans
//
#define HEATER_0_PIN                        PC7
#define HEATER_BED_PIN                      PC6
#ifndef FAN_PIN
  #define FAN_PIN                           PC8
#endif

//
// Temperature Sensors
//
#define TEMP_BED_PIN                        PC5   // Analog Input
#define TEMP_0_PIN                          PC4   // Analog Input

//
// Misc. Functions
//
#define SDSS                                PA4

//
// LCD Pins
//
#if HAS_WIRED_LCD

  #define BEEPER_PIN                        PC9

  #if HAS_MVCNCUI_U8GLIB

    #define DOGLCD_A0                       PA15
    #ifdef pins_v2_20190128
      #define DOGLCD_CS                     PB5
    #else
      #define DOGLCD_CS                     PB7
    #endif

    //#define LCD_CONTRAST_INIT              190
    //#define LCD_SCREEN_ROTATE              180  // 0, 90, 180, 270

  #endif

  // not connected to a pin
  #define SD_DETECT_PIN                     PC3

  #if IS_NEWPANEL
    // The encoder and click button
    #define BTN_EN1                         PC10
    #define BTN_EN2                         PC11
    #define BTN_ENC                         PC12
  #endif

  #ifdef pins_v2_20190128
    #define LCD_RESET_PIN                   PB4
    #ifndef RGB_LED_R_PIN
      #define RGB_LED_R_PIN                 PB0
    #endif
    #ifndef RGB_LED_G_PIN
      #define RGB_LED_G_PIN                 PB6
    #endif
    #ifndef RGB_LED_B_PIN
      #define RGB_LED_B_PIN                 PB7
    #endif
  #else
    #define LCD_RESET_PIN                   PB6
    #ifndef RGB_LED_R_PIN
      #define RGB_LED_R_PIN                 PB3
    #endif
    #ifndef RGB_LED_G_PIN
      #define RGB_LED_G_PIN                 PB4
    #endif
    #ifndef RGB_LED_B_PIN
      #define RGB_LED_B_PIN                 PB5
    #endif
  #endif

#endif
