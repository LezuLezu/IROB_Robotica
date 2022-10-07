#include <Servo.h>

const int POTPIN = A0;
const int SERVOPIN = 4;

int potVal = 0;
Servo Servo1;

void setup() {
  Servo1.attach(SERVOPIN);
  pinMode(POTPIN, INPUT);
}

void loop() {
  int servoPosition = map(analogRead(POTPIN), 0, 1023, 0, 180);
  Servo1.write(servoPosition);
  delay(5000);
}
