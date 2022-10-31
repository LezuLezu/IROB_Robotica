const byte IR_LED = 11;
const int IR_REC = 9;

const int A1B = 3;
const int A1A = 4;

unsigned int rpm;
unsigned int rpmCount;
unsigned long startTime;
unsigned long endTime;
unsigned int inputSpeed;
int speed = 50;

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

//  Set default to 0
  rpm = 0;
  rpmCount = 0;

}
void loop(){
  getUserSpeed();
  
  startTime = millis();
  endTime = startTime;
  while((endTime - startTime) <= 6000){
    controlMotor();
    endTime = millis();    
  }  
  Serial.print("\n\nRPM: ");
  rpm = rpmCount / 6;
  Serial.println(rpm);
  Serial.print("Speed: ");
  Serial.println(speed);
  if(rpm > inputSpeed){
      speed = speed + 10;
  }else if (rpm < inputSpeed){
      speed = speed - 5;
  }else if (rpm == inputSpeed){
    stopMotion();
  }

 restMotor();
  
}

void controlMotor(){
  digitalWrite(A1B, HIGH);
  analogWrite(A1A, speed);
  if(digitalRead(IR_REC) == LOW){
    delay(10);
    if(digitalRead(IR_REC) == LOW){
        rpmCount++;  
    }
  }
}

void restMotor(){
  analogWrite(A1A, 0);
  digitalWrite(A1B, LOW);
  rpmCount = 0;
  delay(3000);
}

void getUserSpeed(){
    while(Serial.available() == 0){
    Serial.println("Pick a speed: 35, 45 or 78");
    int inputSpeed = Serial.parseInt();
    if(inputSpeed == 35 || inputSpeed == 45 || inputSpeed == 78){
      Serial.print("You picked : ");
      Serial.print(inputSpeed);
      Serial.println(" RPM");
    }else{
      Serial.println("Invalid Input");
    }

  }
}

void stopMotion(){
  analogWrite(A1A, 0);
  digitalWrite(A1B, LOW);
  Serial.println("Desired RPM reached");
}
