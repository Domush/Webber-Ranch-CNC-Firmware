/**
 * Modern Vintage CNC Firmware
*/

#include "../inc/mvCNCConfigPre.h"

#if NEED_HEX_PRINT

#include "hex_print.h"
#include "../core/serial.h"

#ifdef CPU_32_BIT
  constexpr int byte_start = 4;
  static char _hex[] = "0x00000000";
#else
  constexpr int byte_start = 0;
  static char _hex[] = "0x0000";
#endif

char* hex_byte(const uint8_t b) {
  _hex[byte_start + 4] = hex_nybble(b >> 4);
  _hex[byte_start + 5] = hex_nybble(b);
  return &_hex[byte_start + 4];
}

inline void _hex_word(const uint16_t w) {
  _hex[byte_start + 2] = hex_nybble(w >> 12);
  _hex[byte_start + 3] = hex_nybble(w >> 8);
  _hex[byte_start + 4] = hex_nybble(w >> 4);
  _hex[byte_start + 5] = hex_nybble(w);
}

char* hex_word(const uint16_t w) {
  _hex_word(w);
  return &_hex[byte_start + 2];
}

#ifdef CPU_32_BIT
  char* hex_long(const uintptr_t l) {
    _hex[2] = hex_nybble(l >> 28);
    _hex[3] = hex_nybble(l >> 24);
    _hex[4] = hex_nybble(l >> 20);
    _hex[5] = hex_nybble(l >> 16);
    _hex_word((uint16_t)(l & 0xFFFF));
    return &_hex[2];
  }
#endif

char* hex_address(const void * const w) {
  #ifdef CPU_32_BIT
    (void)hex_long((uintptr_t)w);
  #else
    (void)hex_word((uintptr_t)w);
  #endif
  return _hex;
}

void print_hex_nybble(const uint8_t n)       { SERIAL_CHAR(hex_nybble(n));  }
void print_hex_byte(const uint8_t b)         { SERIAL_ECHO(hex_byte(b));    }
void print_hex_word(const uint16_t w)        { SERIAL_ECHO(hex_word(w));    }
void print_hex_address(const void * const w) { SERIAL_ECHO(hex_address(w)); }

void print_hex_long(const uint32_t w, const char delimiter) {
  SERIAL_ECHOPGM("0x");
  for (int B = 24; B >= 8; B -= 8) {
    print_hex_byte(w >> B);
    SERIAL_CHAR(delimiter);
  }
  print_hex_byte(w);
}

#endif // NEED_HEX_PRINT
