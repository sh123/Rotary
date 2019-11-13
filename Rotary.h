/*
 * Rotary encoder library for Arduino.
 */

#ifndef Rotary_h
#define Rotary_h

#include "Arduino.h"

// Enable this to emit codes twice per step.
// #define HALF_STEP

// Enable weak pullups
#define ENABLE_PULLUPS

// Values returned by 'process'
// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Counter-clockwise step.
#define DIR_CCW 0x20

// Values returned by 'process_button'
// No action
#define BTN_NONE 0x0
// Button was pressed
#define BTN_PRESSED 0x10
// Button was released
#define BTN_RELEASED 0x20
// Button long-pressed
#define BTN_PRESSED_LONG 0x30
// Button long-released
#define BTN_RELEASED_LONG 0x40

class Rotary
{
  public:
    Rotary(char, char, char);
    unsigned char process();
    unsigned char process_button();
  private:
    unsigned char state;
    unsigned char old_btn_pinstate;
    unsigned char old_btn_state;
    unsigned long btn_pressed_time_ms;
    unsigned char pin1;
    unsigned char pin2;
    unsigned char pin3;
};

#endif
 
