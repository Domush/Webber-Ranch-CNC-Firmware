/**
 * Webber Ranch CNC Firmware
 * Copyright (c) 2021 WRCNCFirmware [https://github.com/Domush/Webber-Ranch-CNC-Firmware]
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

#if NOT_TARGET(__AVR_AT90USB1286__) && (DISABLED(ALLOW_AT90USB1286P) || NOT_TARGET(__AVR_AT90USB1286P__))
  #error "Oops! Select 'Teensy++ 2.0' or 'Printrboard' in 'Tools > Board.'"
#endif

#undef ALLOW_AT90USB1286P