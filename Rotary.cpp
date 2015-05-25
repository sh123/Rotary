/* Rotary encoder handler for arduino.
 *
 * Copyright 2011 Ben Buxton. Licenced under the GNU GPL Version 3.
 * Contact: bb@cactii.net
 *
 */

#include "Arduino.h"
#include "Rotary.h"

/*
 * The below state table has, for each state (row), the new state
 * to set based on the next encoder output. From left to right in,
 * the table, the encoder outputs are 00, 01, 10, 11, and the value
 * in that position is the new state to set.
 */

#define R_START 0x0

#define LONG_PRESS_MS 1000

#ifdef HALF_STEP
// Use the half-step state table (emits a code at 00 and 11)
#define R_CCW_BEGIN 0x1
#define R_CW_BEGIN 0x2
#define R_START_M 0x3
#define R_CW_BEGIN_M 0x4
#define R_CCW_BEGIN_M 0x5
const unsigned char ttable[6][4] = {
  // R_START (00)
  {R_START_M,            R_CW_BEGIN,     R_CCW_BEGIN,  R_START},
  // R_CCW_BEGIN
  {R_START_M | DIR_CCW, R_START,        R_CCW_BEGIN,  R_START},
  // R_CW_BEGIN
  {R_START_M | DIR_CW,  R_CW_BEGIN,     R_START,      R_START},
  // R_START_M (11)
  {R_START_M,            R_CCW_BEGIN_M,  R_CW_BEGIN_M, R_START},
  // R_CW_BEGIN_M
  {R_START_M,            R_START_M,      R_CW_BEGIN_M, R_START | DIR_CW},
  // R_CCW_BEGIN_M
  {R_START_M,            R_CCW_BEGIN_M,  R_START_M,    R_START | DIR_CCW},
};
#else
// Use the full-step state table (emits a code at 00 only)
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

const unsigned char ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};
#endif

/*
 * Constructor. Each arg is the pin number for each encoder contact.
 */
Rotary::Rotary(char _pin1, char _pin2, char _pin3) {
  // Assign variables.
  pin1 = _pin1;
  pin2 = _pin2;
  pin3 = _pin3;
  // Set pins to input.
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT);
#ifdef ENABLE_PULLUPS
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
#endif
  // Initialise state.
  state = R_START;
  old_btn_pinstate = HIGH;
  old_btn_state = BTN_RELEASED;
}

unsigned char Rotary::process() {
  // Grab state of input pins.
  unsigned char pinstate = (digitalRead(pin2) << 1) | digitalRead(pin1);
  // Determine new state from the pins and state table.
  state = ttable[state & 0xf][pinstate];
  // Return emit bits, ie the generated event.
  return state & 0x30;
}

unsigned char Rotary::process_button() {
  // Grab button state
  unsigned char new_btn_state = BTN_NONE;
  unsigned char new_btn_pinstate = digitalRead(pin3);

  switch (old_btn_state) {
    case BTN_NONE:
      break;
    case BTN_PRESSED:
      if (new_btn_pinstate == LOW && old_btn_pinstate == LOW && millis() - btn_pressed_time_ms > LONG_PRESS_MS)
      {
        new_btn_state = BTN_PRESSED_LONG;
      }
      if (new_btn_pinstate == HIGH && old_btn_pinstate == LOW) 
      {
        new_btn_state = BTN_RELEASED;
      }
      break;
    case BTN_PRESSED_LONG:
      if (new_btn_pinstate == HIGH && old_btn_pinstate == LOW) 
      {
        new_btn_state = BTN_RELEASED_LONG;
      }
      break;
    case BTN_RELEASED_LONG: 
      old_btn_state = BTN_RELEASED;
      break;
    case BTN_RELEASED:
      if (new_btn_pinstate == LOW && old_btn_pinstate == HIGH) 
      {
        new_btn_state = BTN_PRESSED;
        btn_pressed_time_ms = millis();
      }
      break;
    default:
      break;
  }
  if (new_btn_state != BTN_NONE) 
  {
    old_btn_state = new_btn_state;
  }
  old_btn_pinstate = new_btn_pinstate;
  return new_btn_state;
}

