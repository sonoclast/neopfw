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
#ifndef _MY_BUTTON_H_
#define _MY_BUTTON_H_

#include <Arduino.h>

#define PIN_BUTTON_A 13
#define PIN_BUTTON_B 11
#define DEBOUNCE_MS 5

namespace my_button {
static volatile uint8_t a_is_pressed = 0;
static volatile uint8_t b_is_pressed = 0;
static volatile uint32_t timestamp_a = millis();
static volatile uint32_t timestamp_b = millis();
static volatile uint8_t count_a = 0;
static volatile uint8_t count_b = 0;

void isrButtonA() {
  if (millis() - timestamp_a > DEBOUNCE_MS) {
    count_a = (count_a + 1) % 2;
    if (count_a == 1) a_is_pressed = 1;
    timestamp_a = millis();
  }
}

void isrButtonB() {
  if (millis() - timestamp_b > DEBOUNCE_MS) {
    count_b = (count_b + 1) % 2;
    if (count_b == 1) b_is_pressed = 1;
    timestamp_b = millis();
  }
}

void init() {
  pinMode(PIN_BUTTON_A, INPUT_PULLUP);
  pinMode(PIN_BUTTON_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_A), isrButtonA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_B), isrButtonB, CHANGE);
}

};

#endif
