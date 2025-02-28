/**
 * Modern Vintage CNC Firmware
*/

/**
 * HAL Timers for Teensy 4.0 (IMXRT1062DVL6A) / 4.1 (IMXRT1062DVJ6A)
 */

#ifdef __IMXRT1062__

#include "../../inc/mvCNCConfig.h"

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  switch (timer_num) {
    case MF_TIMER_STEP:
      CCM_CSCMR1 &= ~CCM_CSCMR1_PERCLK_CLK_SEL; // turn off 24mhz mode
      CCM_CCGR1 |= CCM_CCGR1_GPT1_BUS(CCM_CCGR_ON);

      GPT1_CR = 0;                   // disable timer
      GPT1_SR = 0x3F;                // clear all prior status
      GPT1_PR = GPT1_TIMER_PRESCALE - 1;
      GPT1_CR |= GPT_CR_CLKSRC(1);   //clock selection #1 (peripheral clock = 150 MHz)
      GPT1_CR |= GPT_CR_ENMOD;       //reset count to zero before enabling
      GPT1_CR |= GPT_CR_OM1(1);      // toggle mode
      GPT1_OCR1 = (GPT1_TIMER_RATE / frequency) -1; // Initial compare value
      GPT1_IR = GPT_IR_OF1IE;        // Compare3 value
      GPT1_CR |= GPT_CR_EN;          //enable GPT2 counting at 150 MHz

      OUT_WRITE(15, HIGH);
      attachInterruptVector(IRQ_GPT1, &stepTC_Handler);
      NVIC_SET_PRIORITY(IRQ_GPT1, 16);
      break;
    case MF_TIMER_TEMP:
      CCM_CSCMR1 &= ~CCM_CSCMR1_PERCLK_CLK_SEL; // turn off 24mhz mode
      CCM_CCGR0 |= CCM_CCGR0_GPT2_BUS(CCM_CCGR_ON);

      GPT2_CR = 0;                   // disable timer
      GPT2_SR = 0x3F;                // clear all prior status
      GPT2_PR = GPT2_TIMER_PRESCALE - 1;
      GPT2_CR |= GPT_CR_CLKSRC(1);   //clock selection #1 (peripheral clock = 150 MHz)
      GPT2_CR |= GPT_CR_ENMOD;       //reset count to zero before enabling
      GPT2_CR |= GPT_CR_OM1(1);      // toggle mode
      GPT2_OCR1 = (GPT2_TIMER_RATE / frequency) -1; // Initial compare value
      GPT2_IR = GPT_IR_OF1IE;        // Compare3 value
      GPT2_CR |= GPT_CR_EN;          //enable GPT2 counting at 150 MHz

      OUT_WRITE(14, HIGH);
      attachInterruptVector(IRQ_GPT2, &tempTC_Handler);
      NVIC_SET_PRIORITY(IRQ_GPT2, 32);
      break;
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: NVIC_ENABLE_IRQ(IRQ_GPT1); break;
    case MF_TIMER_TEMP: NVIC_ENABLE_IRQ(IRQ_GPT2); break;
  }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: NVIC_DISABLE_IRQ(IRQ_GPT1); break;
    case MF_TIMER_TEMP: NVIC_DISABLE_IRQ(IRQ_GPT2); break;
  }

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  asm volatile("dsb");
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: return (NVIC_IS_ENABLED(IRQ_GPT1));
    case MF_TIMER_TEMP: return (NVIC_IS_ENABLED(IRQ_GPT2));
  }
  return false;
}

void HAL_timer_isr_prologue(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: GPT1_SR = GPT_IR_OF1IE; break; // clear OF3 bit
    case MF_TIMER_TEMP: GPT2_SR = GPT_IR_OF1IE; break; // clear OF3 bit
  }
  asm volatile("dsb");
}

#endif // __IMXRT1062__
