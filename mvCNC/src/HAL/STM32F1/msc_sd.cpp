/**
 * Modern Vintage CNC Firmware
 *
 * Copyright (c) 2020 mvCNCFirmware [https://github.com/Domush/mvCNC-Modern-Vintage-CNC-Firmware]
 * Copyright (c) 2019 BigTreeTech [https://github.com/bigtreetech]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifdef __STM32F1__

#include "../../inc/mvCNCConfigPre.h"

#if HAS_SD_HOST_DRIVE

#include "msc_sd.h"
#include "SPI.h"
#include "usb_reg_map.h"

#define PRODUCT_ID 0x29

USBMassStorage mvCNCMSC;
Serial1Class<USBCompositeSerial> mvCNCCompositeSerial(true);

#include "../../inc/mvCNCConfig.h"

#if SD_CONNECTION_IS(ONBOARD)

  #include "onboard_sd.h"

  static bool MSC_Write(const uint8_t *writebuff, uint32_t startSector, uint16_t numSectors) {
    return (disk_write(0, writebuff, startSector, numSectors) == RES_OK);
  }
  static bool MSC_Read(uint8_t *readbuff, uint32_t startSector, uint16_t numSectors) {
    return (disk_read(0, readbuff, startSector, numSectors) == RES_OK);
  }

#endif

#if ENABLED(EMERGENCY_PARSER)

  // The original callback is not called (no way to retrieve address).
  // That callback detects a special STM32 reset sequence: this functionality is not essential
  // as M997 achieves the same.
  void my_rx_callback(unsigned int, void*) {
    // max length of 16 is enough to contain all emergency commands
    uint8 buf[16];

    //rx is usbSerialPart.endpoints[2]
    uint16 len = usb_get_ep_rx_count(usbSerialPart.endpoints[2].address);
    uint32 total = composite_cdcacm_data_available();

    if (len == 0 || total == 0 || !WITHIN(total, len, COUNT(buf)))
      return;

    // cannot get character by character due to bug in composite_cdcacm_peek_ex
    len = composite_cdcacm_peek(buf, total);

    for (uint32 i = 0; i < len; i++)
      emergency_parser.update(mvCNCCompositeSerial.emergency_state, buf[i+total-len]);
  }

#endif

void MSC_SD_init() {
  USBComposite.setProductId(PRODUCT_ID);
  // Just set mvCNCCompositeSerial enabled to true
  // because when mvCNCCompositeSerial.begin() is used in setup()
  // it clears all USBComposite devices.
  mvCNCCompositeSerial.begin();
  USBComposite.end();
  USBComposite.clear();
  // Set api and register mass storage
  #if SD_CONNECTION_IS(ONBOARD)
    uint32_t cardSize;
    if (disk_initialize(0) == RES_OK) {
      if (disk_ioctl(0, GET_SECTOR_COUNT, (void *)(&cardSize)) == RES_OK) {
        mvCNCMSC.setDriveData(0, cardSize, MSC_Read, MSC_Write);
        mvCNCMSC.registerComponent();
      }
    }
  #endif
  // Register composite Serial
  mvCNCCompositeSerial.registerComponent();
  USBComposite.begin();
  #if ENABLED(EMERGENCY_PARSER)
    composite_cdcacm_set_hooks(USBHID_CDCACM_HOOK_RX, my_rx_callback);
  #endif
}

#endif // HAS_SD_HOST_DRIVE
#endif // __STM32F1__
