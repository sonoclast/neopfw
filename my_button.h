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
