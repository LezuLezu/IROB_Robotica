const int A1B = 3;
const int A1A = 4;
const int POTPIN = A1;
int potOld = 0;
int potCurrent = 0;
int potNew = 0;

void setup() {
  // set pinmodes
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  pinMode(POTPIN, INPUT);
  
  digitalWrite(A1B, LOW); // only one direction needed

  Serial.begin(9600);
}

void loop() {
  potCurrent = analogRead(POTPIN);
  delay(500);
  potNew = analogRead(POTPIN);
  if(potCurrent == potNew){
    potOld = potCurrent;
    int speedVal = map(potOld, 0, 1024, 0, 255);
    Serial.println(speedVal);
    analogWrite(A1A, speedVal);      
  }
}
