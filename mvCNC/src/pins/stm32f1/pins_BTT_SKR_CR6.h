/**
 * Modern Vintage CNC Firmware
*/
#pragma once

/**
 * BigTreeTech SKR CR-6 (STM32F103RET6) board pin assignments
 */

#define DEFAULT_MACHINE_NAME "Creality3D"
#define BOARD_INFO_NAME "BTT SKR CR-6"

#include "env_validate.h"

//
// Release PB4 (Z_STEP_PIN) from JTAG NRST role
//
#define DISABLE_DEBUG

//
// USB connect control
//
#define USB_CONNECT_PIN                     PA14
#define USB_CONNECT_INVERTING              false

//
// EEPROM
//

#if NO_EEPROM_SELECTED
  #define I2C_EEPROM
#endif

#if ENABLED(I2C_EEPROM)
  #define IIC_EEPROM_SDA                    PB7
  #define IIC_EEPROM_SCL                    PB6
#define MVCNC_EEPROM_SIZE              0x1000  // 4KB
#elif ENABLED(SDCARD_EEPROM_EMULATION)
#define MVCNC_EEPROM_SIZE              0x1000  // 4KB
#endif

//
// Limit Switches
//

#define X_STOP_PIN                          PC0
#define Y_STOP_PIN                          PC1
#define Z_STOP_PIN                          PC14  // Endstop or Probe

#define FIL_RUNOUT_PIN                      PC15

//
// Probe
//
#ifndef PROBE_TARE_PIN
  #define PROBE_TARE_PIN                    PA1
#endif

#if ENABLED(PROBE_ACTIVATION_SWITCH)
  #ifndef PROBE_ACTIVATION_SWITCH_PIN
    #define PROBE_ACTIVATION_SWITCH_PIN     PC2   // Optoswitch to Enable Z Probe
  #endif
#endif

//
// Steppers
//
#define X_ENABLE_PIN                        PB14
#define X_STEP_PIN                          PB13
#define X_DIR_PIN                           PB12

#define Y_ENABLE_PIN                        PB11
#define Y_STEP_PIN                          PB10
#define Y_DIR_PIN                           PB2

#define Z_ENABLE_PIN                        PB1
#define Z_STEP_PIN                          PB0
#define Z_DIR_PIN                           PC5

#define E0_ENABLE_PIN                       PD2
#define E0_STEP_PIN                         PB3
#define E0_DIR_PIN                          PB4

//
// Temperature Sensors
//
#define TEMP_0_PIN                          PA0   // TH1
#define TEMP_BED_PIN                        PC3   // TB1

//
// Heaters / Fans
//

#define HEATER_0_PIN                        PC8   // HEATER1
#define HEATER_BED_PIN                      PC9   // HOT BED

#define FAN_PIN                             PC6   // FAN
#define FAN_SOFT_PWM_REQUIRED

#define CONTROLLER_FAN_PIN                  PC7

//
// LCD / Controller
//
#if ENABLED(CR10_STOCKDISPLAY)
  #define BTN_ENC                           PA15
  #define BTN_EN1                           PA9
  #define BTN_EN2                           PA10

  #define LCD_PINS_RS                       PB8
  #define LCD_PINS_ENABLE                   PB15
  #define LCD_PINS_D4                       PB9

  #define BEEPER_PIN                        PB5
#endif

#if HAS_TMC_UART
  /**
   * TMC2209 stepper drivers
   * Hardware serial communication ports.
   */
  #define X_HARDWARE_SERIAL  MSerial4
  #define Y_HARDWARE_SERIAL  MSerial4
  #define Z_HARDWARE_SERIAL  MSerial4
  #define E0_HARDWARE_SERIAL MSerial4

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
#endif

//
// SD Card
//

#ifndef SDCARD_CONNECTION
  #define SDCARD_CONNECTION              ONBOARD
#endif

#if SD_CONNECTION_IS(ONBOARD)
  #define SD_DETECT_PIN                     PC4
  #define ONBOARD_SD_CS_PIN                 PA4   // Chip select for "System" SD card
  #define SDSS                 ONBOARD_SD_CS_PIN
#endif

//
// Misc. Functions
//
#define CASE_LIGHT_PIN                      PA13

#ifndef NEOPIXEL_PIN
  #define NEOPIXEL_PIN                      PA8
#endif

#define SUICIDE_PIN                         PC13
#ifndef SUICIDE_PIN_STATE
  #define SUICIDE_PIN_STATE                  LOW
#endif
