/* 
  Copyright October 2023
  Sonoclast, LLC
  sonoclast@sonoclast.com

  This code was written for a 30-count neopixel strip mounted on a helmet and 
  attached to an Adafruit Feather development board augmented with a custom
  featherwing that includes an accelerometer and two buttons for input. Also
  a lipo battery 2-pin JST connector, a neopixel 3-pin JST connect, and two 
  DPST switches that allow for the battery and neopixels to be dis/connected
  independently from the rest of the circuit.

  The neopixel code was adapted from Adafruit's NeoPixel strip test program.

  --------

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

#include <Adafruit_NeoPixel.h>
#include "my_button.h"
#include "mxc4005xc_i2c.h"

#define PIN_NEOPIXEL 12
#define NEOPIXEL_COUNT 30
Adafruit_NeoPixel neopixels(NEOPIXEL_COUNT, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

static volatile uint8_t neopixel_pattern[NEOPIXEL_COUNT][NEOPIXEL_COUNT] = {
  { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 }
};

mxc4005xc_i2c my_accel;

void setup() {
  Serial.begin(115200);

  my_button::init();

  my_accel.init();

  neopixels.begin();            // INITIALIZE NeoPixel object (REQUIRED)
  neopixels.show();             // Turn OFF all pixels ASAP
  neopixels.setBrightness(50);  // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
  // toggle the brightness between low and high
  if (my_button::a_is_pressed == 1) {
    static uint8_t level = 50;
    level += 40;
    if (level > 50) level = 10;
    neopixels.setBrightness(level);
    my_button::a_is_pressed = 0;
  }

  // set the level accelerometer readings
  static int16_t data_x_offset = 0;
  static int16_t data_y_offset = 0;
  if (my_button::b_is_pressed == 1) {
    data_x_offset = my_accel.getX();
    data_y_offset = my_accel.getY();
    my_button::b_is_pressed = 0;
  }

  // toggle the side neopixels on and off so that they flash
  static uint32_t flash_sides_timestamp = millis();
  static uint8_t flash_ms = 100;
  static uint8_t flash_offset = 10;
  static const uint8_t flash_max = 200;
  static const uint8_t flash_min = 20;
  if (millis() - flash_sides_timestamp > flash_ms) {
    if (neopixel_pattern[0][0] == 1) {
      for (int i = 0; i < NEOPIXEL_COUNT; i++) {
        neopixel_pattern[i][0] = 0;
        neopixel_pattern[i][1] = 0;
        neopixel_pattern[i][NEOPIXEL_COUNT - 2] = 0;
        neopixel_pattern[i][NEOPIXEL_COUNT - 1] = 0;
      }
    } else {
      for (int i = 0; i < NEOPIXEL_COUNT; i++) {
        neopixel_pattern[i][0] = 1;
        neopixel_pattern[i][1] = 1;
        neopixel_pattern[i][NEOPIXEL_COUNT - 2] = 1;
        neopixel_pattern[i][NEOPIXEL_COUNT - 1] = 1;
      }
    }
    flash_sides_timestamp = millis();
    flash_ms += flash_offset;
    if (flash_ms >= flash_max) flash_offset = -10;
    else if (flash_ms <= flash_min) flash_offset = 10;
  }

  // cycle the neopixel color at a frequency corresponding to the accelerometer's Y-axis
  int16_t data_y = my_accel.getY() - 512 - data_y_offset;
  data_y *= -1; // invert accel reading
  if (data_y < 0) data_y = 0;
  else if (data_y > 1023) data_y = 1023;
  uint8_t ms = (float)data_y / 1023.0 * 20 + 2;
  static uint32_t color = 0;
  static uint32_t color_timestamp = millis();
  if (millis() - color_timestamp > ms) {
    static uint16_t hue = 0;
    hue = (hue + 200) % 65536;
    color = neopixels.gamma32(neopixels.ColorHSV(hue));
    color_timestamp = millis();
  }

  // move the neopixel pattern corresponding to the accelerometer's X-axis
  static uint32_t refresh_timestamp = millis();
  if (millis() - refresh_timestamp > 10) {
    int16_t data_x = my_accel.getX() + 512 - data_x_offset;
    if (data_x < 0) data_x = 0;
    else if (data_x > 1023) data_x = 1023;
    uint8_t index = (float)data_x / 1023.0 * 29;
    neopixels.clear();
    for (int i = 0; i < NEOPIXEL_COUNT; i++) {
      if (neopixel_pattern[index][i])
        neopixels.setPixelColor(NEOPIXEL_COUNT - 1 - i, color);
    }
    neopixels.show();
    refresh_timestamp = millis();
  }
}
