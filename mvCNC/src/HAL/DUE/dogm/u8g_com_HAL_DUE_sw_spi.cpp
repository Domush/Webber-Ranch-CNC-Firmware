/**
 * Modern Vintage CNC Firmware
*/

/**
 * Based on u8g_com_std_sw_spi.c
 *
 * Universal 8bit Graphics Library
 *
 * Copyright (c) 2015, olikraus@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this list
 *    of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice, this
 *    list of conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef ARDUINO_ARCH_SAM

#include "../../../inc/mvCNCConfigPre.h"

#if HAS_MVCNCUI_U8GLIB && !IS_U8GLIB_ST7920

#include "u8g_com_HAL_DUE_sw_spi_shared.h"

#include "../../shared/Marduino.h"
#include "../../shared/Delay.h"

#include <U8glib-HAL.h>

#if ENABLED(FYSETC_MINI_12864)
  #define SPISEND_SW_DUE u8g_spiSend_sw_DUE_mode_3
#else
  #define SPISEND_SW_DUE u8g_spiSend_sw_DUE_mode_0
#endif

uint8_t u8g_com_HAL_DUE_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  switch (msg) {
    case U8G_COM_MSG_INIT:
      SCK_pPio = g_APinDescription[u8g->pin_list[U8G_PI_SCK]].pPort;
      SCK_dwMask = g_APinDescription[u8g->pin_list[U8G_PI_SCK]].ulPin;
      MOSI_pPio = g_APinDescription[u8g->pin_list[U8G_PI_MOSI]].pPort;
      MOSI_dwMask = g_APinDescription[u8g->pin_list[U8G_PI_MOSI]].ulPin;
      u8g_SetPIOutput_DUE(u8g, U8G_PI_SCK);
      u8g_SetPIOutput_DUE(u8g, U8G_PI_MOSI);
      u8g_SetPIOutput_DUE(u8g, U8G_PI_CS);
      u8g_SetPIOutput_DUE(u8g, U8G_PI_A0);
      if (U8G_PIN_NONE != u8g->pin_list[U8G_PI_RESET]) u8g_SetPIOutput_DUE(u8g, U8G_PI_RESET);
      u8g_SetPILevel_DUE(u8g, U8G_PI_SCK, 0);
      u8g_SetPILevel_DUE(u8g, U8G_PI_MOSI, 0);
      break;

    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_RESET:
      if (U8G_PIN_NONE != u8g->pin_list[U8G_PI_RESET]) u8g_SetPILevel_DUE(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      #if ENABLED(FYSETC_MINI_12864)           // LCD SPI is running mode 3 while SD card is running mode 0
        if (arg_val) {                        //   SCK idle state needs to be set to the proper idle state before
                                               //   the next chip select goes active
          u8g_SetPILevel_DUE(u8g, U8G_PI_SCK, 1);  //set SCK to mode 3 idle state before CS goes active
          u8g_SetPILevel_DUE(u8g, U8G_PI_CS, LOW);
        }
        else {
          u8g_SetPILevel_DUE(u8g, U8G_PI_CS, HIGH);
          u8g_SetPILevel_DUE(u8g, U8G_PI_SCK, 0); //set SCK to mode 0 idle state after CS goes inactive
        }
      #else
        u8g_SetPILevel_DUE(u8g, U8G_PI_CS, !arg_val);
      #endif
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      SPISEND_SW_DUE(arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ: {
        uint8_t *ptr = (uint8_t *)arg_ptr;
        while (arg_val > 0) {
          SPISEND_SW_DUE(*ptr++);
          arg_val--;
        }
      }
      break;

      case U8G_COM_MSG_WRITE_SEQ_P: {
        uint8_t *ptr = (uint8_t *)arg_ptr;
        while (arg_val > 0) {
          SPISEND_SW_DUE(u8g_pgm_read(ptr));
          ptr++;
          arg_val--;
        }
      }
      break;

    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g_SetPILevel_DUE(u8g, U8G_PI_A0, arg_val);
      break;
  }
  return 1;
}

#endif // HAS_MVCNCUI_U8GLIB && !IS_U8GLIB_ST7920
#endif // ARDUINO_ARCH_SAM
