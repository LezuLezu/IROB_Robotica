const int A1B = 3;
const int A1A = 4;

const int IR_LED = 8;
const int IR_REC = 9;

int serialInput = 0;
int validInput[] = {33, 45, 78};
int validInputCount = 3;

unsigned int rpm;
unsigned int speedRPMInput;

volatile unsigned int break_number = 0;
unsigned long time;

const int BLADE_COUNT = 1;

void setup() {
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  digitalWrite(A1B, LOW); // only one direction needed

  pinMode(IR_LED, OUTPUT);
  digitalWrite(IR_LED, HIGH);

//  Serial input
  Serial.begin(9600);

//Interrupt
attachInterrupt(digitalPinToInterrupt(IR_REC), isr_break_count, FALLING);
break_number = 0;
rpm = 0;
}

void loop() {
  controlMotor();
  displayTimeAndRpm();
  while(true){
    delay(1000);
    noInterrupts();
    rpm = (60 * break_number) / BLADE_COUNT;
    controlMotor();
    displayTimeAndRpm();
    break_number = 0;
    rpm = 0;
    interrupts();
  }
}

void controlMotor(){
  Serial.println("Pick a speed; 33, 45 or 78 rpm.");
  if(Serial.available() > 0){
    serialInput = Serial.parseInt();
    if(serialInput == 33 || serialInput == 45 || serialInput == 78){
      Serial.print("You picked ");
      Serial.print(serialInput);
      Serial.println(" RPM");
      speedRPMInput = serialInput;
      analogWrite(A1A, speedRPMInput);
    }else{
      Serial.println("Invalid Input");
    }
  }  
}

void displayTimeAndRpm(){
  time = millis() / 1000;
  Serial.print(time);
  Serial.print(", ");
  Serial.print(rpm);
  Serial.println(" RPM");
}

void isr_break_count() { 
  break_number++;   
}
