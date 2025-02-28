/**
 * Modern Vintage CNC Firmware
*/
#pragma once

#include "../../../libs/W25Qxx.h"

#define HAS_SPI_FLASH_COMPRESSION 1

/**
 * This class manages and optimizes SPI Flash data storage,
 * keeping an internal buffer to write and save full SPI flash
 * pages as needed.
 *
 * Since the data is always in the buffer, the class is also
 * able to support fast on-the-fly RLE compression/decompression.
 *
 * In testing with the current LVGL_UI it compacts 2.9MB of icons
 * (which have lots of runs) down to 370kB!!! As a result the UI
 * refresh rate becomes faster and now all LVGL UI can fit into a
 * tiny 2MB SPI Flash, such as the Chitu Board.
 *
 * == Usage ==
 *
 * Writing:
 *
 * The class keeps an internal buffer that caches data until it
 * fits into a full SPI Flash page. Each time the buffer fills up
 * the page is saved to SPI Flash. Sequential writes are optimal.
 *
 *    SPIFlashStorage.beginWrite(myStartAddress);
 *    while (there is data to write)
 *      SPIFlashStorage.addData(myBuffer, bufferSize);
 *    SPIFlashStorage.endWrite(); // Flush remaining buffer data
 *
 * Reading:
 *
 * When reading, it loads a full page from SPI Flash at once and
 * keeps it in a private SRAM buffer. Data is loaded as needed to
 * fulfill requests. Sequential reads are optimal.
 *
 *    SPIFlashStorage.beginRead(myStartAddress);
 *    while (there is data to read)
 *      SPIFlashStorage.readData(myBuffer, bufferSize);
 *
 * Compression:
 *
 * The biggest advantage of this class is the RLE compression.
 * With compression activated a second buffer holds the compressed
 * data, so when writing data, as this buffer becomes full it is
 * flushed to SPI Flash.
 *
 * The same goes for reading: A compressed page is read from SPI
 * flash, and the data is uncompressed as needed to provide the
 * requested amount of data.
 */
class SPIFlashStorage {
public:
  // Write operation
  static void beginWrite(uint32_t startAddress);
  static void endWrite();
  static void writeData(uint8_t *data, uint16_t size);

  // Read operation
  static void beginRead(uint32_t startAddress);
  static void readData(uint8_t *data, uint16_t size);

  static uint32_t getCurrentPage() { return m_currentPage; }

private:
  static void flushPage();
  static void savePage(uint8_t *buffer);
  static void loadPage(uint8_t *buffer);
  static void readPage();
  static uint16_t inData(uint8_t *data, uint16_t size);
  static uint16_t outData(uint8_t *data, uint16_t size);

  static uint8_t m_pageData[SPI_FLASH_PageSize];
  static uint32_t m_currentPage;
  static uint16_t m_pageDataUsed;
  static uint16_t pageDataFree() { return SPI_FLASH_PageSize - m_pageDataUsed; }
  static uint32_t m_startAddress;
  #if HAS_SPI_FLASH_COMPRESSION
    static uint8_t m_compressedData[SPI_FLASH_PageSize];
    static uint16_t m_compressedDataUsed;
    static uint16_t compressedDataFree() { return SPI_FLASH_PageSize - m_compressedDataUsed; }
  #endif
};

extern SPIFlashStorage SPIFlash;
