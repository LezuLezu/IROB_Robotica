#include "SevenSegmentTM1637.h"

const int DISPLAY_CLK = 3;
const int DISPLAY_DIO = 2;

SevenSegmentTM1637 display(DISPLAY_CLK, DISPLAY_DIO);

void setup() {
  // put your setup code here, to run once:
  display.begin();
  display.setBacklight(100);
  display.clear();
//  display.print("2458");
//  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 9999 ; i++){
    display.print(String(i));
    delay(1000);
  }
}
