/**
 * Modern Vintage CNC Firmware
*/

/**
 * HAL Timers for Teensy 3.2 (MK20DX256)
 */

#ifdef __MK20DX256__

#include "../../inc/mvCNCConfig.h"

/** \brief Instruction Synchronization Barrier
  Instruction Synchronization Barrier flushes the pipeline in the processor,
  so that all instructions following the ISB are fetched from cache or
  memory, after the instruction has been completed.
*/
FORCE_INLINE static void __ISB() {
  __asm__ __volatile__("isb 0xF":::"memory");
}

/** \brief Data Synchronization Barrier
  This function acts as a special kind of Data Memory Barrier.
  It completes when all explicit memory accesses before this instruction complete.
*/
FORCE_INLINE static void __DSB() {
  __asm__ __volatile__("dsb 0xF":::"memory");
}

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  switch (timer_num) {
    case MF_TIMER_STEP:
      FTM0_MODE = FTM_MODE_WPDIS | FTM_MODE_FTMEN;
      FTM0_SC = 0x00; // Set this to zero before changing the modulus
      FTM0_CNT = 0x0000; // Reset the count to zero
      FTM0_MOD = 0xFFFF; // max modulus = 65535
      FTM0_C0V = (FTM0_TIMER_RATE) / frequency; // Initial FTM Channel 0 compare value
      FTM0_SC = (FTM_SC_CLKS(0b1) & FTM_SC_CLKS_MASK) | (FTM_SC_PS(FTM0_TIMER_PRESCALE_BITS) & FTM_SC_PS_MASK); // Bus clock 60MHz divided by prescaler 8
      FTM0_C0SC = FTM_CSC_CHIE | FTM_CSC_MSA | FTM_CSC_ELSA;
      break;
    case MF_TIMER_TEMP:
      FTM1_MODE = FTM_MODE_WPDIS | FTM_MODE_FTMEN; // Disable write protection, Enable FTM1
      FTM1_SC = 0x00; // Set this to zero before changing the modulus
      FTM1_CNT = 0x0000; // Reset the count to zero
      FTM1_MOD = 0xFFFF; // max modulus = 65535
      FTM1_C0V = (FTM1_TIMER_RATE) / frequency; // Initial FTM Channel 0 compare value 65535
      FTM1_SC = (FTM_SC_CLKS(0b1) & FTM_SC_CLKS_MASK) | (FTM_SC_PS(FTM1_TIMER_PRESCALE_BITS) & FTM_SC_PS_MASK); // Bus clock 60MHz divided by prescaler 4
      FTM1_C0SC = FTM_CSC_CHIE | FTM_CSC_MSA | FTM_CSC_ELSA;
      break;
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: NVIC_ENABLE_IRQ(IRQ_FTM0); break;
    case MF_TIMER_TEMP: NVIC_ENABLE_IRQ(IRQ_FTM1); break;
  }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: NVIC_DISABLE_IRQ(IRQ_FTM0); break;
    case MF_TIMER_TEMP: NVIC_DISABLE_IRQ(IRQ_FTM1); break;
  }

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: return NVIC_IS_ENABLED(IRQ_FTM0);
    case MF_TIMER_TEMP: return NVIC_IS_ENABLED(IRQ_FTM1);
  }
  return false;
}

void HAL_timer_isr_prologue(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP:
      FTM0_CNT = 0x0000;
      FTM0_SC &= ~FTM_SC_TOF; // Clear FTM Overflow flag
      FTM0_C0SC &= ~FTM_CSC_CHF; // Clear FTM Channel Compare flag
      break;
    case MF_TIMER_TEMP:
      FTM1_CNT = 0x0000;
      FTM1_SC &= ~FTM_SC_TOF; // Clear FTM Overflow flag
      FTM1_C0SC &= ~FTM_CSC_CHF; // Clear FTM Channel Compare flag
      break;
  }
}

#endif // __MK20DX256__
