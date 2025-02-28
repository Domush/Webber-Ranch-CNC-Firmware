/**
 * Modern Vintage CNC Firmware
*/

/**
 * mvCNCSerial_Due.cpp - Hardware serial library for Arduino DUE
 * Copyright (c) 2017 Eduardo José Tagle. All right reserved
 * Based on mvCNCSerial for AVR, copyright (c) 2006 Nicholas Zambetti.  All right reserved.
 */
#ifdef ARDUINO_ARCH_SAM

#include "../../inc/mvCNCConfig.h"

#include "mvCNCSerial.h"
#include "InterruptVectors.h"
#include "../../mvCNCCore.h"

template<typename Cfg> typename mvCNCSerial<Cfg>::ring_buffer_r mvCNCSerial<Cfg>::rx_buffer = { 0, 0, { 0 } };
template<typename Cfg> typename mvCNCSerial<Cfg>::ring_buffer_t mvCNCSerial<Cfg>::tx_buffer = { 0 };
template<typename Cfg> bool     mvCNCSerial<Cfg>::_written = false;
template<typename Cfg> uint8_t  mvCNCSerial<Cfg>::xon_xoff_state = mvCNCSerial<Cfg>::XON_XOFF_CHAR_SENT | mvCNCSerial<Cfg>::XON_CHAR;
template<typename Cfg> uint8_t  mvCNCSerial<Cfg>::rx_dropped_bytes = 0;
template<typename Cfg> uint8_t  mvCNCSerial<Cfg>::rx_buffer_overruns = 0;
template<typename Cfg> uint8_t  mvCNCSerial<Cfg>::rx_framing_errors = 0;
template<typename Cfg> typename mvCNCSerial<Cfg>::ring_buffer_pos_t mvCNCSerial<Cfg>::rx_max_enqueued = 0;

// A SW memory barrier, to ensure GCC does not overoptimize loops
#define sw_barrier() asm volatile("": : :"memory");

#include "../../feature/e_parser.h"

// (called with RX interrupts disabled)
template<typename Cfg>
FORCE_INLINE void mvCNCSerial<Cfg>::store_rxd_char() {

  static EmergencyParser::State emergency_state; // = EP_RESET

  // Get the tail - Nothing can alter its value while we are at this ISR
  const ring_buffer_pos_t t = rx_buffer.tail;

  // Get the head pointer
  ring_buffer_pos_t h = rx_buffer.head;

  // Get the next element
  ring_buffer_pos_t i = (ring_buffer_pos_t)(h + 1) & (ring_buffer_pos_t)(Cfg::RX_SIZE - 1);

  // Read the character from the USART
  uint8_t c = HWUART->UART_RHR;

  if (Cfg::EMERGENCYPARSER) emergency_parser.update(emergency_state, c);

  // If the character is to be stored at the index just before the tail
  // (such that the head would advance to the current tail), the RX FIFO is
  // full, so don't write the character or advance the head.
  if (i != t) {
    rx_buffer.buffer[h] = c;
    h = i;
  }
  else if (Cfg::DROPPED_RX && !++rx_dropped_bytes)
    --rx_dropped_bytes;

  const ring_buffer_pos_t rx_count = (ring_buffer_pos_t)(h - t) & (ring_buffer_pos_t)(Cfg::RX_SIZE - 1);
  // Calculate count of bytes stored into the RX buffer

  // Keep track of the maximum count of enqueued bytes
  if (Cfg::MAX_RX_QUEUED) NOLESS(rx_max_enqueued, rx_count);

  if (Cfg::XONOFF) {
    // If the last char that was sent was an XON
    if ((xon_xoff_state & XON_XOFF_CHAR_MASK) == XON_CHAR) {

      // Bytes stored into the RX buffer
      const ring_buffer_pos_t rx_count = (ring_buffer_pos_t)(h - t) & (ring_buffer_pos_t)(Cfg::RX_SIZE - 1);

      // If over 12.5% of RX buffer capacity, send XOFF before running out of
      // RX buffer space .. 325 bytes @ 250kbits/s needed to let the host react
      // and stop sending bytes. This translates to 13mS propagation time.
      if (rx_count >= (Cfg::RX_SIZE) / 8) {

        // At this point, definitely no TX interrupt was executing, since the TX isr can't be preempted.
        // Don't enable the TX interrupt here as a means to trigger the XOFF char, because if it happens
        // to be in the middle of trying to disable the RX interrupt in the main program, eventually the
        // enabling of the TX interrupt could be undone. The ONLY reliable thing this can do to ensure
        // the sending of the XOFF char is to send it HERE AND NOW.

        // About to send the XOFF char
        xon_xoff_state = XOFF_CHAR | XON_XOFF_CHAR_SENT;

        // Wait until the TX register becomes empty and send it - Here there could be a problem
        // - While waiting for the TX register to empty, the RX register could receive a new
        //   character. This must also handle that situation!
        uint32_t status;
        while (!((status = HWUART->UART_SR) & UART_SR_TXRDY)) {

          if (status & UART_SR_RXRDY) {
            // We received a char while waiting for the TX buffer to be empty - Receive and process it!

            i = (ring_buffer_pos_t)(h + 1) & (ring_buffer_pos_t)(Cfg::RX_SIZE - 1);

            // Read the character from the USART
            c = HWUART->UART_RHR;

            if (Cfg::EMERGENCYPARSER) emergency_parser.update(emergency_state, c);

            // If the character is to be stored at the index just before the tail
            // (such that the head would advance to the current tail), the FIFO is
            // full, so don't write the character or advance the head.
            if (i != t) {
              rx_buffer.buffer[h] = c;
              h = i;
            }
            else if (Cfg::DROPPED_RX && !++rx_dropped_bytes)
              --rx_dropped_bytes;
          }
          sw_barrier();
        }

        HWUART->UART_THR = XOFF_CHAR;

        // At this point there could be a race condition between the write() function
        // and this sending of the XOFF char. This interrupt could happen between the
        // wait to be empty TX buffer loop and the actual write of the character. Since
        // the TX buffer is full because it's sending the XOFF char, the only way to be
        // sure the write() function will succeed is to wait for the XOFF char to be
        // completely sent. Since an extra character could be received during the wait
        // it must also be handled!
        while (!((status = HWUART->UART_SR) & UART_SR_TXRDY)) {

          if (status & UART_SR_RXRDY) {
            // A char arrived while waiting for the TX buffer to be empty - Receive and process it!

            i = (ring_buffer_pos_t)(h + 1) & (ring_buffer_pos_t)(Cfg::RX_SIZE - 1);

            // Read the character from the USART
            c = HWUART->UART_RHR;

            if (Cfg::EMERGENCYPARSER) emergency_parser.update(emergency_state, c);

            // If the character is to be stored at the index just before the tail
            // (such that the head would advance to the current tail), the FIFO is
            // full, so don't write the character or advance the head.
            if (i != t) {
              rx_buffer.buffer[h] = c;
              h = i;
            }
            else if (Cfg::DROPPED_RX && !++rx_dropped_bytes)
              --rx_dropped_bytes;
          }
          sw_barrier();
        }

        // At this point everything is ready. The write() function won't
        // have any issues writing to the UART TX register if it needs to!
      }
    }
  }

  // Store the new head value
  rx_buffer.head = h;
}

template<typename Cfg>
FORCE_INLINE void mvCNCSerial<Cfg>::_tx_thr_empty_irq() {
  if (Cfg::TX_SIZE > 0) {
    // Read positions
    uint8_t t = tx_buffer.tail;
    const uint8_t h = tx_buffer.head;

    if (Cfg::XONOFF) {
      // If an XON char is pending to be sent, do it now
      if (xon_xoff_state == XON_CHAR) {

        // Send the character
        HWUART->UART_THR = XON_CHAR;

        // Remember we sent it.
        xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;

        // If nothing else to transmit, just disable TX interrupts.
        if (h == t) HWUART->UART_IDR = UART_IDR_TXRDY;

        return;
      }
    }

    // If nothing to transmit, just disable TX interrupts. This could
    // happen as the result of the non atomicity of the disabling of RX
    // interrupts that could end reenabling TX interrupts as a side effect.
    if (h == t) {
      HWUART->UART_IDR = UART_IDR_TXRDY;
      return;
    }

    // There is something to TX, Send the next byte
    const uint8_t c = tx_buffer.buffer[t];
    t = (t + 1) & (Cfg::TX_SIZE - 1);
    HWUART->UART_THR = c;
    tx_buffer.tail = t;

    // Disable interrupts if there is nothing to transmit following this byte
    if (h == t) HWUART->UART_IDR = UART_IDR_TXRDY;
  }
}

template<typename Cfg>
void mvCNCSerial<Cfg>::UART_ISR() {
  const uint32_t status = HWUART->UART_SR;

  // Data received?
  if (status & UART_SR_RXRDY) store_rxd_char();

  if (Cfg::TX_SIZE > 0) {
    // Something to send, and TX interrupts are enabled (meaning something to send)?
    if ((status & UART_SR_TXRDY) && (HWUART->UART_IMR & UART_IMR_TXRDY)) _tx_thr_empty_irq();
  }

  // Acknowledge errors
  if ((status & UART_SR_OVRE) || (status & UART_SR_FRAME)) {
    if (Cfg::DROPPED_RX && (status & UART_SR_OVRE) && !++rx_dropped_bytes) --rx_dropped_bytes;
    if (Cfg::RX_OVERRUNS && (status & UART_SR_OVRE) && !++rx_buffer_overruns) --rx_buffer_overruns;
    if (Cfg::RX_FRAMING_ERRORS && (status & UART_SR_FRAME) && !++rx_framing_errors) --rx_framing_errors;

    // TODO: error reporting outside ISR
    HWUART->UART_CR = UART_CR_RSTSTA;
  }
}

// Public Methods
template<typename Cfg>
void mvCNCSerial<Cfg>::begin(const long baud_setting) {

  // Disable UART interrupt in NVIC
  NVIC_DisableIRQ( HWUART_IRQ );

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();

  // Disable clock
  pmc_disable_periph_clk( HWUART_IRQ_ID );

  // Configure PMC
  pmc_enable_periph_clk( HWUART_IRQ_ID );

  // Disable PDC channel
  HWUART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

  // Reset and disable receiver and transmitter
  HWUART->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

  // Configure mode: 8bit, No parity, 1 bit stop
  HWUART->UART_MR = UART_MR_CHMODE_NORMAL | US_MR_CHRL_8_BIT | US_MR_NBSTOP_1_BIT | UART_MR_PAR_NO;

  // Configure baudrate (asynchronous, no oversampling)
  HWUART->UART_BRGR = (SystemCoreClock / (baud_setting << 4));

  // Configure interrupts
  HWUART->UART_IDR = 0xFFFFFFFF;
  HWUART->UART_IER = UART_IER_RXRDY | UART_IER_OVRE | UART_IER_FRAME;

  // Install interrupt handler
  install_isr(HWUART_IRQ, UART_ISR);

  // Configure priority. We need a very high priority to avoid losing characters
  // and we need to be able to preempt the Stepper ISR and everything else!
  // (this could probably be fixed by using DMA with the Serial port)
  NVIC_SetPriority(HWUART_IRQ, 1);

  // Enable UART interrupt in NVIC
  NVIC_EnableIRQ(HWUART_IRQ);

  // Enable receiver and transmitter
  HWUART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;

  if (Cfg::TX_SIZE > 0) _written = false;
}

template<typename Cfg>
void mvCNCSerial<Cfg>::end() {
  // Disable UART interrupt in NVIC
  NVIC_DisableIRQ( HWUART_IRQ );

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();

  pmc_disable_periph_clk( HWUART_IRQ_ID );
}

template<typename Cfg>
int mvCNCSerial<Cfg>::peek() {
  const int v = rx_buffer.head == rx_buffer.tail ? -1 : rx_buffer.buffer[rx_buffer.tail];
  return v;
}

template<typename Cfg>
int mvCNCSerial<Cfg>::read() {

  const ring_buffer_pos_t h = rx_buffer.head;
  ring_buffer_pos_t t = rx_buffer.tail;

  if (h == t) return -1;

  int v = rx_buffer.buffer[t];
  t = (ring_buffer_pos_t)(t + 1) & (Cfg::RX_SIZE - 1);

  // Advance tail
  rx_buffer.tail = t;

  if (Cfg::XONOFF) {
    // If the XOFF char was sent, or about to be sent...
    if ((xon_xoff_state & XON_XOFF_CHAR_MASK) == XOFF_CHAR) {
      // Get count of bytes in the RX buffer
      const ring_buffer_pos_t rx_count = (ring_buffer_pos_t)(h - t) & (ring_buffer_pos_t)(Cfg::RX_SIZE - 1);
      // When below 10% of RX buffer capacity, send XON before running out of RX buffer bytes
      if (rx_count < (Cfg::RX_SIZE) / 10) {
        if (Cfg::TX_SIZE > 0) {
          // Signal we want an XON character to be sent.
          xon_xoff_state = XON_CHAR;
          // Enable TX isr.
          HWUART->UART_IER = UART_IER_TXRDY;
        }
        else {
          // If not using TX interrupts, we must send the XON char now
          xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;
          while (!(HWUART->UART_SR & UART_SR_TXRDY)) sw_barrier();
          HWUART->UART_THR = XON_CHAR;
        }
      }
    }
  }

  return v;
}

template<typename Cfg>
typename mvCNCSerial<Cfg>::ring_buffer_pos_t mvCNCSerial<Cfg>::available() {
  const ring_buffer_pos_t h = rx_buffer.head, t = rx_buffer.tail;
  return (ring_buffer_pos_t)(Cfg::RX_SIZE + h - t) & (Cfg::RX_SIZE - 1);
}

template<typename Cfg>
void mvCNCSerial<Cfg>::flush() {
  rx_buffer.tail = rx_buffer.head;

  if (Cfg::XONOFF) {
    if ((xon_xoff_state & XON_XOFF_CHAR_MASK) == XOFF_CHAR) {
      if (Cfg::TX_SIZE > 0) {
        // Signal we want an XON character to be sent.
        xon_xoff_state = XON_CHAR;
        // Enable TX isr.
        HWUART->UART_IER = UART_IER_TXRDY;
      }
      else {
        // If not using TX interrupts, we must send the XON char now
        xon_xoff_state = XON_CHAR | XON_XOFF_CHAR_SENT;
        while (!(HWUART->UART_SR & UART_SR_TXRDY)) sw_barrier();
        HWUART->UART_THR = XON_CHAR;
      }
    }
  }
}

template<typename Cfg>
size_t mvCNCSerial<Cfg>::write(const uint8_t c) {
  _written = true;

  if (Cfg::TX_SIZE == 0) {
    while (!(HWUART->UART_SR & UART_SR_TXRDY)) sw_barrier();
    HWUART->UART_THR = c;
  }
  else {

    // If the TX interrupts are disabled and the data register
    // is empty, just write the byte to the data register and
    // be done. This shortcut helps significantly improve the
    // effective datarate at high (>500kbit/s) bitrates, where
    // interrupt overhead becomes a slowdown.
    // Yes, there is a race condition between the sending of the
    // XOFF char at the RX isr, but it is properly handled there
    if (!(HWUART->UART_IMR & UART_IMR_TXRDY) && (HWUART->UART_SR & UART_SR_TXRDY)) {
      HWUART->UART_THR = c;
      return 1;
    }

    const uint8_t i = (tx_buffer.head + 1) & (Cfg::TX_SIZE - 1);

    // If global interrupts are disabled (as the result of being called from an ISR)...
    if (!ISRS_ENABLED()) {

      // Make room by polling if it is possible to transmit, and do so!
      while (i == tx_buffer.tail) {
        // If we can transmit another byte, do it.
        if (HWUART->UART_SR & UART_SR_TXRDY) _tx_thr_empty_irq();
        // Make sure compiler rereads tx_buffer.tail
        sw_barrier();
      }
    }
    else {
      // Interrupts are enabled, just wait until there is space
      while (i == tx_buffer.tail) sw_barrier();
    }

    // Store new char. head is always safe to move
    tx_buffer.buffer[tx_buffer.head] = c;
    tx_buffer.head = i;

    // Enable TX isr - Non atomic, but it will eventually enable TX isr
    HWUART->UART_IER = UART_IER_TXRDY;
  }
  return 1;
}

template<typename Cfg>
void mvCNCSerial<Cfg>::flushTX() {
  // TX

  if (Cfg::TX_SIZE == 0) {
    // No bytes written, no need to flush. This special case is needed since there's
    // no way to force the TXC (transmit complete) bit to 1 during initialization.
    if (!_written) return;

    // Wait until everything was transmitted
    while (!(HWUART->UART_SR & UART_SR_TXEMPTY)) sw_barrier();

    // At this point nothing is queued anymore (DRIE is disabled) and
    // the hardware finished transmission (TXC is set).

  }
  else {
    // If we have never written a byte, no need to flush. This special
    // case is needed since there is no way to force the TXC (transmit
    // complete) bit to 1 during initialization
    if (!_written) return;

    // If global interrupts are disabled (as the result of being called from an ISR)...
    if (!ISRS_ENABLED()) {

      // Wait until everything was transmitted - We must do polling, as interrupts are disabled
      while (tx_buffer.head != tx_buffer.tail || !(HWUART->UART_SR & UART_SR_TXEMPTY)) {
        // If there is more space, send an extra character
        if (HWUART->UART_SR & UART_SR_TXRDY) _tx_thr_empty_irq();
        sw_barrier();
      }

    }
    else {
      // Wait until everything was transmitted
      while (tx_buffer.head != tx_buffer.tail || !(HWUART->UART_SR & UART_SR_TXEMPTY)) sw_barrier();
    }

    // At this point nothing is queued anymore (DRIE is disabled) and
    // the hardware finished transmission (TXC is set).
  }
}


// If not using the USB port as serial port
#if defined(SERIAL_PORT) && SERIAL_PORT >= 0
  template class mvCNCSerial< mvCNCSerialCfg<SERIAL_PORT> >;
  MSerialT1 customizedSerial1(mvCNCSerialCfg<SERIAL_PORT>::EMERGENCYPARSER);
#endif

#if defined(SERIAL_PORT_2) && SERIAL_PORT_2 >= 0
  template class mvCNCSerial< mvCNCSerialCfg<SERIAL_PORT_2> >;
  MSerialT2 customizedSerial2(mvCNCSerialCfg<SERIAL_PORT_2>::EMERGENCYPARSER);
#endif

#if defined(SERIAL_PORT_3) && SERIAL_PORT_3 >= 0
  template class mvCNCSerial< mvCNCSerialCfg<SERIAL_PORT_3> >;
  MSerialT3 customizedSerial3(mvCNCSerialCfg<SERIAL_PORT_3>::EMERGENCYPARSER);
#endif

#endif // ARDUINO_ARCH_SAM
