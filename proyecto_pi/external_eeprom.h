/* 
 * Proyecto PI. An inteligent boiler control system
 * Copyright (C) 2014  Rafael Bailón-Ruiz <rafaelbailon en ieee punto org> 
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EXTERNAL_EEPROM_H
#define EXTERNAL_EEPROM_H

#define EEPROM_24FC512_MAXBYTES 64000
#define EEPROM_DEVICE_ADDRESS 0x50

const byte MAGIC_CODE[] = {0x60, 0x0D, 0xF0, 0x0D};
#define MAGIC_CODE_ADDR 0x00

#define ORDER_ADDR 0x03

#endif
