/**
 * Modern Vintage CNC Firmware
*/
#pragma once

/**
 * MKS GEN L V2 – Arduino Mega2560 with RAMPS v1.4 pin assignments
 */

#if HOTENDS > 2 || E_STEPPERS > 2
  #error "MKS GEN L V2 supports up to 2 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_INFO_NAME "MKS GEN L V2"

//
// Heaters / Fans
//
// Power outputs EFBF or EFBE
#define MOSFET_D_PIN                           7

//
// CS Pins wired to avoid conflict with the LCD
// See https://www.thingiverse.com/asset:66604
//

#ifndef X_CS_PIN
  #define X_CS_PIN                            63
#endif

#ifndef Y_CS_PIN
  #define Y_CS_PIN                            64
#endif

#ifndef Z_CS_PIN
  #define Z_CS_PIN                            65
#endif

#ifndef E0_CS_PIN
  #define E0_CS_PIN                           66
#endif

#ifndef E1_CS_PIN
  #define E1_CS_PIN                           21
#endif

// TMC2130 Diag Pins (currently just for reference)
#define X_DIAG_PIN                             3
#define Y_DIAG_PIN                            14
#define Z_DIAG_PIN                            18
#define E0_DIAG_PIN                            2
#define E1_DIAG_PIN                           15

#ifndef SERVO1_PIN
  #define SERVO1_PIN                          12
#endif
#ifndef SERVO2_PIN
  #define SERVO2_PIN                          39
#endif
#ifndef SERVO3_PIN
  #define SERVO3_PIN                          32
#endif

#ifndef E1_SERIAL_TX_PIN
  #define E1_SERIAL_TX_PIN                    20
#endif
#ifndef E1_SERIAL_RX_PIN
  #define E1_SERIAL_RX_PIN                    21
#endif

#include "pins_RAMPS.h"
