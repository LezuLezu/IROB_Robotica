const int SENSORPIN = A0;
const int LED = 4;
const int minHZ = 1;
const int maxHZ = 200;
void setup() {
  Serial.begin(9600);
  pinMode(SENSORPIN, INPUT);
  pinMode(LED, OUTPUT);
}
void loop() {
  byte timeDelay = map(analogRead(SENSORPIN), 0, 1023, minHZ, maxHZ);
  digitalWrite(LED, !digitalRead(LED));
  delay(timeDelay);
}
