#include "LedControl.h"

#include "SevenSegmentTM1637.h"

const int DISPLAY_CLK = 3;
const int DISPLAY_DIO = 2;

SevenSegmentTM1637 display(DISPLAY_CLK, DISPLAY_DIO);

const int DATA = 12;
const int CLK = 11;
const int LOAD = 10;
const int AANT = 1; 

const int smiley[8] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100,
 };
 const int neutral[8] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10111101,
  B10000001,
  B01000010,
  B00111100,
 };
const int sad[8] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10011001,
  B10100101,
  B01000010,
  B00111100,
};
 LedControl matrix = LedControl(DATA, CLK, LOAD, AANT);

void setup() {
  // put your setup code here, to run once:
  matrix.shutdown(0, false);
  matrix.setIntensity(0,8);
  matrix.clearDisplay(0);
  Serial.begin(9600);
//  Serial.println("Naar loop");
//  showArray(matrix, smiley);
  display.begin();
  display.setBacklight(100);
  display.clear();
 
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 900; i++){
    display.print(String(i));
    delay(10);
    if(i >= 0 && i <= 300){
      Serial.println("SAD");
      showArray(matrix, sad);
    }else if(i >= 301 && i <= 600){
      Serial.println("MEH");
      showArray(matrix, neutral);
    }else if(i >= 601 && i <=900){
      Serial.println("HAPPY");
      showArray(matrix, smiley);
    }
  }
  
}

void showArray(LedControl lc, const int arr[8]){
  for(int i = 0; i < 8; i++){
    lc.setRow(0, i, arr[i]);
//    Serial.println(i);    
  }
}
