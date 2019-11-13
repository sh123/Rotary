/*
    Rotary Encoder - Polling Example

    The circuit:
    * encoder pin A to Arduino pin 2
    * encoder pin B to Arduino pin 3
    * encoder button pin to Arduino pin 4
    * encoder ground pin to ground (GND)

*/

#include <Rotary.h>

Rotary r = Rotary(2, 3, 4);

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned char result = r.process();
  if (result) {
    Serial.println(result == DIR_CW ? "Right" : "Left");
  }

  unsigned char result_btn = r.process_button();
  if (result_btn) {
    Serial.println(result == BTN_PRESSED ? "Pressed" : "Released");
  }
}
