const byte IR_LED = 11;
const int IR_REC = 9;

const int A1B = 3;
const int A1A = 4;

unsigned int rpm;
unsigned int rpmCount;
unsigned long startTime;
unsigned long endTime;
const int desiredRPM = 33;
int speed = 30;

void setup() {
  Serial.begin(9600);
  
//  Define pinmodes
  pinMode(IR_LED,OUTPUT);
  pinMode(IR_REC, INPUT);
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
//  digitalWrite(A1B, LOW);

  //Create 338khz blink on ir led
  TCCR2A = _BV(COM2A0) | _BV(WGM21); 
  TCCR2B = _BV(CS20);
  OCR2A = 209;
  rpm = 0;
  rpmCount = 0;

}
void loop(){
  startTime = millis();
  endTime = startTime;
  while((endTime - startTime) <= 6000){
    digitalWrite(A1B, HIGH);
    analogWrite(A1A, speed);
    if(digitalRead(IR_REC) == LOW){
      rpmCount++;
      Serial.print("\n\nRPM: ");
      rpm = rpmCount / 6;
      Serial.println(rpm);
      Serial.print("Speed: ");
      Serial.println(speed);
    }   
    endTime = millis();    
  }  
  
  if(rpm < desiredRPM){
      speed = speed + 10;
    }else if (rpm > desiredRPM){
      speed = speed - 5;
    }
  analogWrite(A1A, 0);
  rpmCount = 0;
  delay(3000);  
}
