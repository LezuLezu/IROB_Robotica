const int SENSORPIN = A0;
const int LED = 4;

int sensorValueOld = 0;
int sensorValueCurrent = 0;
int sensorValueNew = 0;

const double STEP = 5.0 / 1023;

bool printedHigh = false;
bool printedLow = false;

void setup() {
  Serial.begin(9600);
  pinMode(SENSORPIN, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  sensorValueCurrent = analogRead(SENSORPIN);
  delay(500);
  sensorValueNew = analogRead(SENSORPIN);
  if (sensorValueCurrent == sensorValueNew){
    if(sensorValueCurrent != sensorValueOld){
      sensorValueOld = sensorValueCurrent;

      if( sensorValueCurrent > 555 && printedHigh == false){
        Serial.print("High : ");
        Serial.println(sensorValueCurrent);
        printedHigh = true;
        printedLow = false;
      }
      else if (sensorValueCurrent <= 555 && printedLow == false){
        Serial.print("Low : ");
        Serial.println(sensorValueCurrent);
        printedHigh = false;
        printedLow = true;
      }
    }
  }
}
