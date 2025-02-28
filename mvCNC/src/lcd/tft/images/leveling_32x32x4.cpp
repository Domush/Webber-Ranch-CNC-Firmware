/**
 * Modern Vintage CNC Firmware
*/

#include "../../../inc/mvCNCConfigPre.h"

#if HAS_GRAPHICAL_TFT

extern const uint8_t leveling_32x32x4[512] = {
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x87, 0x89, 0x99, 0x99, 0x99, 0x99, 0x87, 0x78, 0x88, 0x77, 0x78, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x87, 0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE6, 0x88, 0x87, 0x7B, 0x77, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x87, 0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC4, 0x88, 0x87, 0x8F, 0x97, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x77, 0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0x88, 0x77, 0xEF, 0xF5, 0x88, 0x88, 0x88,
  0x88, 0x87, 0x77, 0xAF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC5, 0x88, 0x7A, 0xFF, 0xFC, 0x78, 0x88, 0x88,
  0x88, 0x87, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x97, 0x7A, 0x88, 0x88, 0x58, 0x88, 0x88,
  0x88, 0x87, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x66, 0x87, 0x45, 0x55, 0x57, 0x88, 0x88,
  0x88, 0x87, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x76, 0x88, 0x77, 0x78, 0x88, 0x88, 0x88,
  0x88, 0x87, 0x85, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xC5, 0x56, 0x7C, 0xFF, 0xFE, 0x67, 0x88, 0x88,
  0x88, 0x88, 0x86, 0x55, 0xEF, 0xFF, 0xFF, 0xF9, 0x45, 0x57, 0x86, 0xFF, 0xF5, 0x57, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x87, 0x4C, 0xFF, 0xFF, 0x74, 0x57, 0x88, 0x87, 0x8F, 0xA4, 0x68, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x84, 0x9F, 0xD5, 0x45, 0x78, 0x88, 0x88, 0x7C, 0x55, 0x88, 0x88, 0x88,
  0x87, 0x77, 0x77, 0x77, 0x88, 0x65, 0x55, 0x68, 0x87, 0x77, 0x77, 0x86, 0x57, 0x87, 0x77, 0x78,
  0x77, 0x77, 0x77, 0x77, 0x77, 0x76, 0x56, 0x77, 0x77, 0x77, 0x77, 0x77, 0x67, 0x77, 0x77, 0x77,
  0x7B, 0xFE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEF, 0xB7,
  0x7C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xB6,
  0x7C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xB5,
  0x78, 0x67, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x66,
  0x87, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x67,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88
};

#endif // HAS_GRAPHICAL_TFT
