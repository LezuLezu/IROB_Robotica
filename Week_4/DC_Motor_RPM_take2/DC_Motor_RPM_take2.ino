const int A1B = 3;
const int A1A = 4;

const byte IR_LED = 11;
const int IR_REC = 9;

unsigned int rpm;
unsigned int rpmCount;
bool done = false; 
unsigned long time;

void setup() {

  Serial.begin(9600);
  Serial.println("Press ! to end the program");
  Serial.println("Please enter the desired speed of the motor.");
  Serial.println("Must be a value between of 33, 45 or 78.");

//  Define pinmodes
  pinMode(IR_LED,OUTPUT);
  pinMode(IR_REC, INPUT);

  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  digitalWrite(A1B, LOW); // only one direction needed


//Create 338khz blink on ir led
  TCCR2A = _BV(COM2A0) | _BV(WGM21); 
  TCCR2B = _BV(CS20);
  OCR2A = 209;

//  Create interrupt on 
  attachInterrupt(digitalPinToInterrupt(IR_REC), rpmCounter, LOW);
  rpm = 0;
  rpmCount = 0;
}

void loop() {
  display_time_and_rpm();
  start_motor();

    while(!done) {     
      delay(1000);
      // Don't process interrupts during this calculation
      noInterrupts();  
      rpm = (60*rpmCount);
      start_motor();
      // Display the time and rpm
      display_time_and_rpm();
   
      // End program if sentinel is entered 
      end_program();    
       
      rpm = 0;
      rpmCount = 0 ;
   
      // Restart interrupts
      interrupts();
  }

}

void start_motor() {
 
  // Wait for the user to enter the speed of the motor
  while (Serial.available() == 0){};
 
  // Activate the motor
  int speed = Serial.parseInt();
  if (speed == 35 || speed == 45 || speed == 78) {
    Serial.print("You picked ");
    Serial.print(speed);
    Serial.println(" RPM");
    analogWrite(A1A, speed);
  }
}

void display_time_and_rpm() {   
  // Capture the time and covert to seconds
  time = millis() / 1000; 
  // Display the time
  Serial.print(time); 
  Serial.print(" , ");
  // Println so the next line begins on a new line
  // Display the rpm
  Serial.println(rpm);   
}

void rpmCounter(){
  rpmCount++;
}

void end_program() {
   
  // Used for reading data from the serial monitor
  char ch;
 
  // Check to see if ! is available to be read
  if (Serial.available()) {     
   
    // Read the character
    // Serial.read() returns the first (oldest) character in the buffer 
    // and removes that byte of data from the buffer
    ch = Serial.read();    
 
    // End the program if an exclamation point is entered in the
    // serial monitor
    if (ch == '!') {
      done = true;  
 
      // Turn off the motor
      analogWrite(A1A, 0);
 
      // Turn off the IR LED
      digitalWrite(IR_LED, LOW);
       
      Serial.println("Finished recording RPM. Goodbye.");
    }
  }    
}
