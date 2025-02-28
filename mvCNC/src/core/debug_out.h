/**
 * Modern Vintage CNC Firmware
*/

//
// Serial aliases for debugging.
// Include this header after defining DEBUG_OUT
//  (or not) in a given .cpp file
//

#undef DEBUG_SECTION
#undef DEBUG_ECHO_START
#undef DEBUG_ERROR_START
#undef DEBUG_CHAR
#undef DEBUG_ECHO
#undef DEBUG_DECIMAL
#undef DEBUG_ECHO_F
#undef DEBUG_ECHOLN
#undef DEBUG_ECHOPGM
#undef DEBUG_ECHOLNPGM
#undef DEBUG_ECHOF
#undef DEBUG_ECHOLNF
#undef DEBUG_ECHOPGM_P
#undef DEBUG_ECHOLNPGM_P
#undef DEBUG_ECHOPAIR_F
#undef DEBUG_ECHOPAIR_F_P
#undef DEBUG_ECHOLNPAIR_F
#undef DEBUG_ECHOLNPAIR_F_P
#undef DEBUG_ECHO_MSG
#undef DEBUG_ERROR_MSG
#undef DEBUG_EOL
#undef DEBUG_FLUSH
#undef DEBUG_POS
#undef DEBUG_XYZ
#undef DEBUG_DELAY
#undef DEBUG_SYNCHRONIZE

#if DEBUG_OUT

  #include "debug_section.h"
  #define DEBUG_SECTION(N,S,D)    SectionLog N(F(S),D)

  #define DEBUG_ECHO_START        SERIAL_ECHO_START
  #define DEBUG_ERROR_START       SERIAL_ERROR_START
  #define DEBUG_CHAR              SERIAL_CHAR
  #define DEBUG_ECHO              SERIAL_ECHO
  #define DEBUG_DECIMAL           SERIAL_DECIMAL
  #define DEBUG_ECHO_F            SERIAL_ECHO_F
  #define DEBUG_ECHOLN            SERIAL_ECHOLN
  #define DEBUG_ECHOPGM           SERIAL_ECHOPGM
  #define DEBUG_ECHOLNPGM         SERIAL_ECHOLNPGM
  #define DEBUG_ECHOF             SERIAL_ECHOF
  #define DEBUG_ECHOLNF           SERIAL_ECHOLNF
  #define DEBUG_ECHOPGM           SERIAL_ECHOPGM
  #define DEBUG_ECHOPGM_P         SERIAL_ECHOPGM_P
  #define DEBUG_ECHOPAIR_F        SERIAL_ECHOPAIR_F
  #define DEBUG_ECHOPAIR_F_P      SERIAL_ECHOPAIR_F_P
  #define DEBUG_ECHOLNPGM         SERIAL_ECHOLNPGM
  #define DEBUG_ECHOLNPGM_P       SERIAL_ECHOLNPGM_P
  #define DEBUG_ECHOLNPAIR_F      SERIAL_ECHOLNPAIR_F
  #define DEBUG_ECHOLNPAIR_F_P    SERIAL_ECHOLNPAIR_F_P
  #define DEBUG_ECHO_MSG          SERIAL_ECHO_MSG
  #define DEBUG_ERROR_MSG         SERIAL_ERROR_MSG
  #define DEBUG_EOL               SERIAL_EOL
  #define DEBUG_FLUSH             SERIAL_FLUSH
  #define DEBUG_POS               SERIAL_POS
  #define DEBUG_XYZ               SERIAL_XYZ
  #define DEBUG_DELAY(ms)         serial_delay(ms)
  #define DEBUG_SYNCHRONIZE()     planner.synchronize()

#else

  #define DEBUG_SECTION(...)        NOOP
  #define DEBUG_ECHO_START()        NOOP
  #define DEBUG_ERROR_START()       NOOP
  #define DEBUG_CHAR(...)           NOOP
  #define DEBUG_ECHO(...)           NOOP
  #define DEBUG_DECIMAL(...)        NOOP
  #define DEBUG_ECHO_F(...)         NOOP
  #define DEBUG_ECHOLN(...)         NOOP
  #define DEBUG_ECHOPGM(...)        NOOP
  #define DEBUG_ECHOLNPGM(...)      NOOP
  #define DEBUG_ECHOF(...)          NOOP
  #define DEBUG_ECHOLNF(...)        NOOP
  #define DEBUG_ECHOPGM_P(...)      NOOP
  #define DEBUG_ECHOLNPGM_P(...)    NOOP
  #define DEBUG_ECHOPAIR_F(...)     NOOP
  #define DEBUG_ECHOPAIR_F_P(...)   NOOP
  #define DEBUG_ECHOLNPAIR_F(...)   NOOP
  #define DEBUG_ECHOLNPAIR_F_P(...) NOOP
  #define DEBUG_ECHO_MSG(...)       NOOP
  #define DEBUG_ERROR_MSG(...)      NOOP
  #define DEBUG_EOL()               NOOP
  #define DEBUG_FLUSH()             NOOP
  #define DEBUG_POS(...)            NOOP
  #define DEBUG_XYZ(...)            NOOP
  #define DEBUG_DELAY(...)          NOOP
  #define DEBUG_SYNCHRONIZE()       NOOP

#endif

#undef DEBUG_OUT
