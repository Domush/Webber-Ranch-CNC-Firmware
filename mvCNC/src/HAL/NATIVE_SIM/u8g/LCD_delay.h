/**
 * Modern Vintage CNC Firmware
 * Copyright (c) 2021 mvCNCFirmware [https://github.com/Domush/mvCNC-Modern-Vintage-CNC-Firmware]
 *
 * Based on Marlin and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
#pragma once

/**
 * LCD delay routines - used by all the drivers.
 *
 * These are based on the LPC1768 routines.
 *
 * Couldn't just call exact copies because the overhead
 * results in a one microsecond delay taking about 4µS.
 */

#ifdef __cplusplus
  extern "C" {
#endif

void U8g_delay(int msec);
void u8g_MicroDelay();
void u8g_10MicroDelay();

#ifdef __cplusplus
  }
#endif
