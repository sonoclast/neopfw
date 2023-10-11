/*
  Copyright October 2023
  Sonoclast, LLC
  sonoclast@sonoclast.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _MXC4005XC_I2C_H
#define _MXC4005XC_I2C_H

#include <Wire.h>

#define I2C_PORT Wire

#define I2C_ACCEL_ADDR_WRITE 0x15
#define I2C_ACCEL_ADDR_READ 0x95

#define I2C_ACCEL_XOUT_MSB 0x03
#define I2C_ACCEL_XOUT_LSB 0x04
#define I2C_ACCEL_YOUT_MSB 0x05
#define I2C_ACCEL_YOUT_LSB 0x06
#define I2C_ACCEL_ZOUT_MSB 0x07
#define I2C_ACCEL_ZOUT_LSB 0x08

class mxc4005xc_i2c {
public:
  mxc4005xc_i2c();
  void init();
  // output is between -1024 to 1023 for leaning, and -2048 to 2047 for shaking
  int16_t getX(); // X is along the long dimension of PCB
  int16_t getY(); // Y is along the narrow dimension of the PCB
  int16_t getZ(); // Z is orthogonal to the PCB
private:
  int8_t readOneByte(uint8_t addr);
  int16_t readTwoBytes(uint8_t addr);
};



#endif