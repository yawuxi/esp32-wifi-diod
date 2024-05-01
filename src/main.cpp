#include <Arduino.h>

void setup() {
  Serial.begin(152600);
}

void loop() {
  delay(500);
  Serial.println("HELLO");
}

