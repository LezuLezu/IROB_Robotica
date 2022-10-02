#include "LedControl.h"
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

 const int heart[8] = {
  B01000100,
  B01100110,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000,
 };
const int sqaure[8] = {
  B11111111,
  B10000001,
  B10111101,
  B10100101,
  B10100101,
  B10111101,
  B10000001,
  B11111111,
};
 LedControl matrix = LedControl(DATA, CLK, LOAD, AANT);

void setup() {
  // put your setup code here, to run once:
  matrix.shutdown(0, false);
  matrix.setIntensity(0,8);
  matrix.clearDisplay(0);
  Serial.begin(9600);
  Serial.println("Naar loop");
//  showArray(matrix, smiley);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  showArray(matrix, smiley);
  delay(1000);
  showArray(matrix, heart);
  delay(1000);
  showArray(matrix, sqaure);
  delay(1000);
}

void showArray(LedControl lc, const int arr[8]){
  for(int i = 0; i < 8; i++){
    lc.setRow(0, i, arr[i]);
    Serial.println(i);    
  }
}
