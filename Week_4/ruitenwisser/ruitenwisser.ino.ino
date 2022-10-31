#include <Servo.h>

const int POTPIN = A0;
const int SERVOPIN = 4;

int pos = 0;
Servo Servo1;

void setup() {
  Servo1.attach(SERVOPIN);
  pinMode(POTPIN, INPUT);
}

void loop() {
  for(pos =0; pos <= 180; pos += 1){
      byte potDelay = map(analogRead(POTPIN), 0, 1023, 0, 800);
      Servo1.write(pos);
      delay(potDelay);
  }
  for(pos = 180; pos >= 0; pos -= 1){
      byte potDelay = map(analogRead(POTPIN), 0, 1023, 0, 800);
      Servo1.write(pos);
      delay(potDelay);
    }
}
