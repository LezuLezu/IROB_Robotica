const int SENSORPIN = A0;
const int LED = 4;

int sensorValueOld = 0;
int sensorValueCurrent = 0;
int sensorValueNew = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(SENSORPIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  sensorValueCurrent = analogRead(SENSORPIN);
  delay(500);
  sensorValueNew = analogRead(SENSORPIN);
  if (sensorValueCurrent == sensorValueNew){
    if(sensorValueCurrent != sensorValueOld){
      sensorValueOld = sensorValueCurrent;
      Serial.println(sensorValueCurrent);
      if(sensorValueCurrent < 200){
        digitalWrite(LED, HIGH);
      }else{
        digitalWrite(LED, LOW);
      }
    }
  }  
}
