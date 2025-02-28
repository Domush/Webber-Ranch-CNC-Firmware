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
#include "HAL_MinSerial.h"

#if ENABLED(POSTMORTEM_DEBUGGING)

void HAL_min_serial_init_default() {}
void HAL_min_serial_out_default(char ch) { SERIAL_CHAR(ch); }
void (*HAL_min_serial_init)() = &HAL_min_serial_init_default;
void (*HAL_min_serial_out)(char) = &HAL_min_serial_out_default;

bool MinSerial::force_using_default_output = false;

#endif
