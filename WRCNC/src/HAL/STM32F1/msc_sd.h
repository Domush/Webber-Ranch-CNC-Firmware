/**
 * Webber Ranch CNC Firmware
 *
 * Copyright (c) 2020 WRCNCFirmware [https://github.com/Domush/Webber-Ranch-CNC-Firmware]
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
#pragma once

#include <USBComposite.h>

#include "../../inc/WRCNCConfigPre.h"
#include "../../core/serial_hook.h"

extern USBMassStorage WRCNCMSC;
extern Serial1Class<USBCompositeSerial> WRCNCCompositeSerial;

void MSC_SD_init();