/**
 * Modern Vintage CNC Firmware
*/

/**
 * HAL SPI for Teensy 3.5 (MK64FX512) and Teensy 3.6 (MK66FX1M0)
 */

#if defined(__MK64FX512__) || defined(__MK66FX1M0__)

#include "../../inc/mvCNCConfig.h"
#include "HAL.h"

#include <SPI.h>
#include <pins_arduino.h>
#include "spi_pins.h"

static SPISettings spiConfig;

void spiBegin() {
  #if PIN_EXISTS(SD_SS)
    OUT_WRITE(SD_SS_PIN, HIGH);
  #endif
  SET_OUTPUT(SD_SCK_PIN);
  SET_INPUT(SD_MISO_PIN);
  SET_OUTPUT(SD_MOSI_PIN);

  #if 0 && DISABLED(SOFTWARE_SPI)
    // set SS high - may be chip select for another SPI device
    #if SET_SPI_SS_HIGH
      WRITE(SD_SS_PIN, HIGH);
    #endif
    // set a default rate
    spiInit(SPI_HALF_SPEED); // 1
  #endif
}

void spiInit(uint8_t spiRate) {
  // Use mvCNC data-rates
  uint32_t clock;
  switch (spiRate) {
  case SPI_FULL_SPEED:    clock = 10000000; break;
  case SPI_HALF_SPEED:    clock =  5000000; break;
  case SPI_QUARTER_SPEED: clock =  2500000; break;
  case SPI_EIGHTH_SPEED:  clock =  1250000; break;
  case SPI_SPEED_5:       clock =   625000; break;
  case SPI_SPEED_6:       clock =   312500; break;
  default:
    clock = 4000000; // Default from the SPI library
  }
  spiConfig = SPISettings(clock, MSBFIRST, SPI_MODE0);
  SPI.begin();
}

uint8_t spiRec() {
  SPI.beginTransaction(spiConfig);
  uint8_t returnByte = SPI.transfer(0xFF);
  SPI.endTransaction();
  return returnByte;
  //SPDR = 0xFF;
  //while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
  //return SPDR;
}

void spiRead(uint8_t *buf, uint16_t nbyte) {
  SPI.beginTransaction(spiConfig);
  SPI.transfer(buf, nbyte);
  SPI.endTransaction();
  //if (nbyte-- == 0) return;
  //  SPDR = 0xFF;
  //for (uint16_t i = 0; i < nbyte; i++) {
  //  while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
  //  buf[i] = SPDR;
  //  SPDR = 0xFF;
  //}
  //while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
  //buf[nbyte] = SPDR;
}

void spiSend(uint8_t b) {
  SPI.beginTransaction(spiConfig);
  SPI.transfer(b);
  SPI.endTransaction();
  //SPDR = b;
  //while (!TEST(SPSR, SPIF)) { /* Intentionally left empty */ }
}

void spiSendBlock(uint8_t token, const uint8_t *buf) {
  SPI.beginTransaction(spiConfig);
  SPDR = token;
  for (uint16_t i = 0; i < 512; i += 2) {
    while (!TEST(SPSR, SPIF)) { /* nada */ };
    SPDR = buf[i];
    while (!TEST(SPSR, SPIF)) { /* nada */ };
    SPDR = buf[i + 1];
  }
  while (!TEST(SPSR, SPIF)) { /* nada */ };
  SPI.endTransaction();
}

// Begin SPI transaction, set clock, bit order, data mode
void spiBeginTransaction(uint32_t spiClock, uint8_t bitOrder, uint8_t dataMode) {
  spiConfig = SPISettings(spiClock, bitOrder, dataMode);
  SPI.beginTransaction(spiConfig);
}

#endif // __MK64FX512__ || __MK66FX1M0__
