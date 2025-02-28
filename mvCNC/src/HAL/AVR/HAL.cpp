/**
 * Modern Vintage CNC Firmware
*/
#ifdef __AVR__

#include "../../inc/mvCNCConfig.h"
#include "HAL.h"

#ifdef USBCON
  DefaultSerial1 MSerial0(false, Serial);
  #ifdef BLUETOOTH
    BTSerial btSerial(false, bluetoothSerial);
  #endif
#endif

// ------------------------
// Public Variables
// ------------------------

// Don't initialize/override variable (which would happen in .init4)
uint8_t reset_reason __attribute__((section(".noinit")));

// ------------------------
// Public functions
// ------------------------

__attribute__((naked))             // Don't output function pro- and epilogue
__attribute__((used))              // Output the function, even if "not used"
__attribute__((section(".init3"))) // Put in an early user definable section
void HAL_save_reset_reason() {
  #if ENABLED(OPTIBOOT_RESET_REASON)
    __asm__ __volatile__(
      A("STS %0, r2")
      : "=m"(reset_reason)
    );
  #else
    reset_reason = MCUSR;
  #endif

  // Clear within 16ms since WDRF bit enables a 16ms watchdog timer -> Boot loop
  MCUSR = 0;
  wdt_disable();
}

void HAL_init() {
  // Init Servo Pins
  #define INIT_SERVO(N) OUT_WRITE(SERVO##N##_PIN, LOW)
  #if HAS_SERVO_0
    INIT_SERVO(0);
  #endif
  #if HAS_SERVO_1
    INIT_SERVO(1);
  #endif
  #if HAS_SERVO_2
    INIT_SERVO(2);
  #endif
  #if HAS_SERVO_3
    INIT_SERVO(3);
  #endif

  init_pwm_timers();   // Init user timers to default frequency - 1000HZ
}

void HAL_reboot() {
  #if ENABLED(USE_WATCHDOG)
    while (1) { /* run out the watchdog */ }
  #else
    void (*resetFunc)() = 0;  // Declare resetFunc() at address 0
    resetFunc();              // Jump to address 0
  #endif
}

#if ENABLED(SDSUPPORT)

  #include "../../sd/SdFatUtil.h"
  int freeMemory() { return SdFatUtil::FreeRam(); }

#else // !SDSUPPORT

extern "C" {
  extern char __bss_end;
  extern char __heap_start;
  extern void* __brkval;

  int freeMemory() {
    int free_memory;
    if ((int)__brkval == 0)
      free_memory = ((int)&free_memory) - ((int)&__bss_end);
    else
      free_memory = ((int)&free_memory) - ((int)__brkval);
    return free_memory;
  }
}

#endif // !SDSUPPORT

#endif // __AVR__
