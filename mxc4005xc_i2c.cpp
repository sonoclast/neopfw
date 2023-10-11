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
#include "mxc4005xc_i2c.h"

mxc4005xc_i2c::mxc4005xc_i2c() {
}

void mxc4005xc_i2c::init() {
  I2C_PORT.begin();
}

int16_t mxc4005xc_i2c::getX() {
  int16_t data = readTwoBytes(I2C_ACCEL_XOUT_MSB);
  return data;
}

int16_t mxc4005xc_i2c::getY() {
  int16_t data = readTwoBytes(I2C_ACCEL_YOUT_MSB);
  return data;
}

int16_t mxc4005xc_i2c::getZ() {
  int16_t data = readTwoBytes(I2C_ACCEL_ZOUT_MSB);
  return data;
}

int8_t mxc4005xc_i2c::readOneByte(uint8_t addr) {
  int8_t data = 0;
  I2C_PORT.beginTransmission(I2C_ACCEL_ADDR_WRITE);
  I2C_PORT.write(addr);
  I2C_PORT.endTransmission();
  I2C_PORT.requestFrom(I2C_ACCEL_ADDR_READ, 1);
  while (I2C_PORT.available()) {
    data = I2C_PORT.read();
  }
  return data;
}

int16_t mxc4005xc_i2c::readTwoBytes(uint8_t addr) {
  int16_t data = 0;
  I2C_PORT.beginTransmission(I2C_ACCEL_ADDR_WRITE);
  I2C_PORT.write(addr);
  I2C_PORT.endTransmission();
  I2C_PORT.requestFrom(I2C_ACCEL_ADDR_READ, 2);
  while (!I2C_PORT.available()) {}
  data = I2C_PORT.read();
  while (!I2C_PORT.available()) {}
  data = (data << 4) | (I2C_PORT.read() >> 4);
  // output is in 2's complement format with a range of -2048 to +2047
  if (data > 2047) {
    data -= 4096;
  }
  return data;
}