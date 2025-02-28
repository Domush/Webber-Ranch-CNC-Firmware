/**
 * Modern Vintage CNC Firmware
*/

#include "../../../inc/mvCNCConfigPre.h"

#if HAS_GRAPHICAL_TFT

extern const uint8_t pause_64x64x4[2048] = {
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x77, 0x77, 0x77, 0x77, 0x78, 0x88, 0x88, 0x88, 0x88, 0x87, 0x77, 0x77, 0x77, 0x77, 0x78, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xD8, 0x78, 0x88, 0x88, 0x87, 0x8D, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0x78, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE8, 0x78, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x77, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0x67, 0x88, 0x88, 0x87, 0x8E, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xD7, 0x67, 0x88, 0x88, 0x87, 0x8D, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x77, 0x77, 0x77, 0x77, 0x76, 0x67, 0x88, 0x88, 0x88, 0x88, 0x77, 0x77, 0x77, 0x77, 0x76, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x66, 0x66, 0x66, 0x66, 0x66, 0x67, 0x88, 0x88, 0x88, 0x87, 0x66, 0x66, 0x66, 0x66, 0x66, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x66, 0x66, 0x66, 0x66, 0x66, 0x77, 0x88, 0x88, 0x88, 0x87, 0x76, 0x66, 0x66, 0x66, 0x66, 0x67, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x77, 0x77, 0x77, 0x77, 0x88, 0x88, 0x88, 0x88, 0x88, 0x87, 0x77, 0x77, 0x77, 0x77, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88
};

#endif // HAS_GRAPHICAL_TFT
