/**
 * Modern Vintage CNC Firmware
*/
#pragma once

#include "../inc/mvCNCConfigPre.h"
#include "../core/types.h"

// Format uint8_t (0-100) as rj string with 123% / _12% / __1% format
const char* pcttostrpctrj(const uint8_t i);

// Convert uint8_t (0-255) to a percentage, format as above
const char* ui8tostr4pctrj(const uint8_t i);

// Convert uint8_t to string with 12 format
const char* ui8tostr2(const uint8_t x);

// Convert uint8_t to string with 123 format
const char* ui8tostr3rj(const uint8_t i);

// Convert int8_t to string with 123 format
const char* i8tostr3rj(const int8_t x);

#if HAS_PRINT_PROGRESS_PERMYRIAD
  // Convert 16-bit unsigned permyriad value to percent: 100 / 23 / 23.4 / 3.45
  const char* permyriadtostr4(const uint16_t xx);
#endif

// Convert uint16_t to string with 12345 format
const char* ui16tostr5rj(const uint16_t x);

// Convert uint16_t to string with 1234 format
const char* ui16tostr4rj(const uint16_t x);

// Convert uint16_t to string with 123 format
const char* ui16tostr3rj(const uint16_t x);

// Convert int16_t to string with 123 format
const char* i16tostr3rj(const int16_t x);

// Convert unsigned int to lj string with 123 format
const char* i16tostr3left(const int16_t xx);

// Convert signed int to rj string with _123, -123, _-12, or __-1 format
const char* i16tostr4signrj(const int16_t x);

// Convert unsigned float to string with 1.2 format
const char* ftostr11ns(const_float_t x);

// Convert unsigned float to string with 1.23 format
const char* ftostr12ns(const_float_t x);

// Convert unsigned float to string with 12.3 format
const char* ftostr31ns(const_float_t x);

// Convert unsigned float to string with 123.4 format
const char* ftostr41ns(const_float_t x);

// Convert signed float to fixed-length string with 12.34 / _2.34 / -2.34 or -23.45 / 123.45 format
const char* ftostr42_52(const_float_t x);

// Convert signed float to fixed-length string with 023.45 / -23.45 format
const char* ftostr52(const_float_t x);

// Convert signed float to fixed-length string with 12.345 / -2.345 or 023.456 / -23.456 format
const char* ftostr53_63(const_float_t x);

// Convert signed float to fixed-length string with 023.456 / -23.456 format
const char* ftostr63(const_float_t x);

// Convert float to fixed-length string with +12.3 / -12.3 format
const char* ftostr31sign(const_float_t x);

// Convert float to fixed-length string with +123.4 / -123.4 format
const char* ftostr41sign(const_float_t x);

// Convert signed float to string (6 digit) with -1.234 / _0.000 / +1.234 format
const char* ftostr43sign(const_float_t x, char plus=' ');

// Convert signed float to string (5 digit) with -1.2345 / _0.0000 / +1.2345 format
const char* ftostr54sign(const_float_t x, char plus=' ');

// Convert unsigned float to rj string with 12345 format
const char* ftostr5rj(const_float_t x);

// Convert signed float to string with +1234.5 format
const char* ftostr51sign(const_float_t x);

// Convert signed float to space-padded string with -_23.4_ format
const char* ftostr52sp(const_float_t x);

// Convert signed float to string with +123.45 format
const char* ftostr52sign(const_float_t x);

// Convert signed float to string with +12.345 format
const char* ftostr53sign(const_float_t f);

// Convert unsigned float to string with 1234.5 format omitting trailing zeros
const char* ftostr51rj(const_float_t x);

// Convert float to rj string with 123 or -12 format
FORCE_INLINE const char* ftostr3(const_float_t x) { return i16tostr3rj(int16_t(x + (x < 0 ? -0.5f : 0.5f))); }

#if ENABLED(LCD_DECIMAL_SMALL_XY)
  // Convert float to rj string with 1234, _123, 12.3, _1.2, -123, _-12, or -1.2 format
  const char* ftostr4sign(const_float_t fx);
#else
  // Convert float to rj string with 1234, _123, -123, __12, _-12, ___1, or __-1 format
  FORCE_INLINE const char* ftostr4sign(const_float_t x) { return i16tostr4signrj(int16_t(x + (x < 0 ? -0.5f : 0.5f))); }
#endif
