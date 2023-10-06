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