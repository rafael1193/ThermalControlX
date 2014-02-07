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

/* 
 *  Use the I2C bus with EEPROM 24LC64 
 *  Sketch:    eeprom.pde
 *  
 *  Author: hkhijhe
 *  Date: 01/10/2010
 * 
 *   
 */

#include <Wire.h>
#include "external_eeprom.h"

void i2c_eeprom_write_byte(unsigned int eeaddress, byte data)
{
  int rdata = data;
  Wire.beginTransmission(EEPROM_DEVICE_ADDRESS);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page(unsigned int eeaddresspage, byte* data, byte length)
{
  Wire.beginTransmission(EEPROM_DEVICE_ADDRESS);
  Wire.write((int)(eeaddresspage >> 8)); // MSB
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB
  byte c;
  for ( c = 0; c < length; c++)
    Wire.write(data[c]);
  Wire.endTransmission();
}

byte i2c_eeprom_read_byte(unsigned int eeaddress)
{
  byte rdata = 0xFF;
  Wire.beginTransmission(EEPROM_DEVICE_ADDRESS);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(EEPROM_DEVICE_ADDRESS,1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

// maybe let's not read more than 30 or 32 bytes at a time!
void i2c_eeprom_read_buffer(unsigned int eeaddress, byte *buffer, int length)
{
  Wire.beginTransmission(EEPROM_DEVICE_ADDRESS);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(EEPROM_DEVICE_ADDRESS,length);
  int c = 0;
  for ( c = 0; c < length; c++ )
    if (Wire.available()) buffer[c] = Wire.read();
}
