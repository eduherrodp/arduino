#include <Arduino.h>

// put function declarations here:
#define pinLed 2 // 30 pines
// #define pinLed 9 // 38 pines

void setup() {
  pinMode(pinLed, OUTPUT);
}

void loop() {
  digitalWrite(pinLed, HIGH);
  delay(1000);
  digitalWrite(pinLed, LOW);
  delay(1000);
}
 