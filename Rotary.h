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

class Rotary
{
  public:
    Rotary(char, char, char);
    unsigned char process();
    unsigned char process_button();
  private:
    unsigned char state;
    unsigned char state_btn;
    unsigned char pin1;
    unsigned char pin2;
    unsigned char pin3;
};

#endif
 
