/**
 * Modern Vintage CNC Firmware
*/

#include "serial.h"
#include "../inc/mvCNCConfig.h"

#if HAS_ETHERNET
  #include "../feature/ethernet.h"
#endif

uint8_t mvcnc_debug_flags = MVCNC_DEBUG_NONE;

// Commonly-used strings in serial output
PGMSTR(NUL_STR,   "");   PGMSTR(SP_P_STR, " P");  PGMSTR(SP_T_STR, " T");
PGMSTR(X_STR,     "X");  PGMSTR(Y_STR,     "Y");  PGMSTR(Z_STR,     "Z");  PGMSTR(E_STR,     "E");
PGMSTR(X_LBL,     "X:"); PGMSTR(Y_LBL,     "Y:"); PGMSTR(Z_LBL,     "Z:"); PGMSTR(E_LBL,     "E:");
PGMSTR(SP_A_STR, " A");  PGMSTR(SP_B_STR, " B");  PGMSTR(SP_C_STR, " C");
PGMSTR(SP_X_STR, " X");  PGMSTR(SP_Y_STR, " Y");  PGMSTR(SP_Z_STR, " Z");  PGMSTR(SP_E_STR, " E");
PGMSTR(SP_X_LBL, " X:"); PGMSTR(SP_Y_LBL, " Y:"); PGMSTR(SP_Z_LBL, " Z:"); PGMSTR(SP_E_LBL, " E:");
PGMSTR(I_STR, STR_I);     PGMSTR(J_STR, STR_J);     PGMSTR(K_STR, STR_K);
PGMSTR(I_LBL, STR_I ":"); PGMSTR(J_LBL, STR_J ":"); PGMSTR(K_LBL, STR_K ":");
PGMSTR(SP_I_STR, " " STR_I);     PGMSTR(SP_J_STR, " " STR_J);     PGMSTR(SP_K_STR, " " STR_K);
PGMSTR(SP_I_LBL, " " STR_I ":"); PGMSTR(SP_J_LBL, " " STR_J ":"); PGMSTR(SP_K_LBL, " " STR_K ":");

// Hook Meatpack if it's enabled on the first leaf
#if ENABLED(MEATPACK_ON_SERIAL_PORT_1)
  SerialLeafT1 mpSerial1(false, _SERIAL_LEAF_1);
#endif
#if ENABLED(MEATPACK_ON_SERIAL_PORT_2)
  SerialLeafT2 mpSerial2(false, _SERIAL_LEAF_2);
#endif
#if ENABLED(MEATPACK_ON_SERIAL_PORT_3)
  SerialLeafT3 mpSerial3(false, _SERIAL_LEAF_3);
#endif

// Step 2: For multiserial, handle the second serial port as well
#if HAS_MULTI_SERIAL
  #if HAS_ETHERNET
    // We need a definition here
    SerialLeafT2 msSerial2(ethernet.have_telnet_client, MYSERIAL2, false);
  #endif

  #define __S_LEAF(N) ,SERIAL_LEAF_##N
  #define _S_LEAF(N) __S_LEAF(N)

  SerialOutputT multiSerial( SERIAL_LEAF_1 REPEAT_S(2, INCREMENT(NUM_SERIAL), _S_LEAF) );

  #undef __S_LEAF
  #undef _S_LEAF

#endif

void serial_print_P(PGM_P str) {
  while (const char c = pgm_read_byte(str++)) SERIAL_CHAR(c);
}

void serial_echo_start()  { static PGMSTR(echomagic, "echo:"); serial_print_P(echomagic); }
void serial_error_start() { static PGMSTR(errormagic, "Error:"); serial_print_P(errormagic); }

void serial_spaces(uint8_t count) { count *= (PROPORTIONAL_FONT_RATIO); while (count--) SERIAL_CHAR(' '); }

void serial_ternary(const bool onoff, FSTR_P const pre, FSTR_P const on, FSTR_P const off, FSTR_P const post/*=nullptr*/) {
  if (pre) serial_print(pre);
  serial_print(onoff ? on : off);
  if (post) serial_print(post);
}
void serialprint_onoff(const bool onoff) { serial_print(onoff ? F(STR_ON) : F(STR_OFF)); }
void serialprintln_onoff(const bool onoff) { serialprint_onoff(onoff); SERIAL_EOL(); }
void serialprint_truefalse(const bool tf) { serial_print(tf ? F("true") : F("false")); }

void print_bin(uint16_t val) {
  for (uint8_t i = 16; i--;) {
    SERIAL_CHAR('0' + TEST(val, i));
    if (!(i & 0x3) && i) SERIAL_CHAR(' ');
  }
}

void print_pos(LINEAR_AXIS_ARGS(const_float_t), FSTR_P const prefix/*=nullptr*/, FSTR_P const suffix/*=nullptr*/) {
  if (prefix) serial_print(prefix);
  SERIAL_ECHOPGM_P(
    LIST_N(DOUBLE(LINEAR_AXES), SP_X_STR, x, SP_Y_STR, y, SP_Z_STR, z, SP_I_STR, i, SP_J_STR, j, SP_K_STR, k)
  );
  if (suffix) serial_print(suffix); else SERIAL_EOL();
}
