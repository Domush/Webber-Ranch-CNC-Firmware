/**
 * Modern Vintage CNC Firmware
 *
 * Copyright (c) 2020 mvCNCFirmware [https://github.com/Domush/mvCNC-Modern-Vintage-CNC-Firmware]
 * SAMD51 HAL developed by Giuliano Zaro (AKA GMagician)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifdef __SAMD51__

#include "../../inc/mvCNCConfig.h"

#if ENABLED(QSPI_EEPROM)

#include "../shared/eeprom_api.h"

#include "QSPIFlash.h"

static bool initialized;

size_t PersistentStore::capacity() { return qspi.size(); }

bool PersistentStore::access_start() {
  if (!initialized) {
    qspi.begin();
    initialized = true;
  }
  return true;
}

bool PersistentStore::access_finish() {
  qspi.flush();
  return true;
}

bool PersistentStore::write_data(int &pos, const uint8_t *value, size_t size, uint16_t *crc) {
  while (size--) {
    const uint8_t v = *value;
    qspi.writeByte(pos, v);
    crc16(crc, &v, 1);
    pos++;
    value++;
  }
  return false;
}

bool PersistentStore::read_data(int &pos, uint8_t *value, size_t size, uint16_t *crc, const bool writing/*=true*/) {
  while (size--) {
    uint8_t c = qspi.readByte(pos);
    if (writing) *value = c;
    crc16(crc, &c, 1);
    pos++;
    value++;
  }
  return false;
}

#endif // QSPI_EEPROM
#endif // __SAMD51__
