
const int LED = 4;
const int POTPIN = A0;


void setup() {
  pinMode(LED, OUTPUT);
  pinMode(POTPIN, INPUT);
  Serial.begin(9600);

//  Interrupt

}

void loop() {
  int ledBrightness = map(analogRead(POTPIN), 1, 2023, 0, 255);
  delay(100);
}
