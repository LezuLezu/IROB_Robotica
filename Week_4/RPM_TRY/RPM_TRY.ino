/**
 * In this program, we develop an Arduino-Based Optical Tachometer 
 * that measures the speed of a brushless propeller.
 * 
 * The Infrared LED is connected to pin 13.
 * The Infrared Phototransistor is connected to pin 2 (interrupts).
 * 
 * @version 1.0 2019-02-18
 * @author Addison Sears-Collins
 * https://automaticaddison.com/how-to-develop-an-arduino-based-optical-tachometer/
 */
  
// Assign a name to the DC motor pin on the Arduino Uno
const unsigned int MOTOR_PIN = 4;
 
// Assign a name to the infrared LED pin on the Arduino Uno
const unsigned int IR_LED = 8;
 
// The number of blades on the propeller. Adjust accordingly.
const unsigned int BLADE_COUNT = 1;
 
// Volatile keyword is used with interrupts
// This variable is subject to change inside an interrupt
// service routine
volatile unsigned int break_number = 0;
 
// Flag used to stop the program
bool done = false;
 
// Used for capturing the time
unsigned long time;
 
// Used for capturing the rpm (revolutions per minute)
unsigned int rpm;
 
/**
 *  Function runs only once, after each powerup or reset of the Arduino Uno
 */
void setup() {
 
  // Open the serial port and set the data transmission rate to 9600 bits 
  // per second. 9600 is the default baud rate for Arduino Uno.
  Serial.begin(9600);
 
  // Show a welcome message as human-readable ASCII text
  Serial.println("PROPELLER RPM PROGRAM");
  Serial.println("This program transmits the time and RPM of a propeller.");
  Serial.println("Created by Addison Sears-Collins");
  Serial.println("");
  Serial.println("Press ! to end the program");
  Serial.println("");
  Serial.println("Please enter the desired speed of the motor.");
  Serial.println("Must be a value between 100 and 255.");
  Serial.println("");
  Serial.println("TIME, RPM");
 
  // The Infrared phototransistor is connected to pin 2.
  // Interrupt triggers when signal goes from HIGH to LOW
  attachInterrupt(digitalPinToInterrupt(9), isr_break_count, FALLING); 
 
  // Turn on the IR Led
  pinMode(IR_LED, OUTPUT);
  digitalWrite(IR_LED, HIGH);
 
  // Enable output for the motor
  pinMode(MOTOR_PIN, OUTPUT);
 
  break_number = 0;
  rpm = 0;  
  
}
 
/**
 *  Main function
 */
void loop() {
 
  display_time_and_rpm();
   
  start_motor();
 
  while(!done) {    
 
    // Update time and rpm every second
    delay(1000);
 
    // Don't process interrupts during this calculation
    noInterrupts();
 
    // Calculate the RPM. If a 3-blade propeller, 3 breaks
    // per second would yield 1 rpm, which is 60 rpm. 
    rpm = (60 * break_number) / BLADE_COUNT;
 
    // Display the time and rpm
    display_time_and_rpm();
 
    // End program if sentinel is entered 
    end_program();    
     
    break_number = 0;
    rpm = 0;
 
    // Restart interrupts
    interrupts();
  }
   
  // Do nothing
  while (true) {}
}
 
/**
  * This function starts the motor
  */
void start_motor() {
 
  // Wait for the user to enter the speed of the motor
  while (Serial.available() == 0){};
 
  // Activate the motor
  int speed = Serial.parseInt();
  if (speed >= 100 && speed <= 255) {
    analogWrite(MOTOR_PIN, speed);
  }
}
 
/**
  * Interrupt service routine.
  * This function counts the number of interrupts
  */
void isr_break_count() {
 
  break_number++;
   
}
 
/**
  * Function displays the time and rpm
  */
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
 
/**
  * This function ends the program
  */
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
      analogWrite(MOTOR_PIN, 0);
 
      // Turn off the IR LED
      digitalWrite(IR_LED, LOW);
       
      Serial.println("Finished recording RPM. Goodbye.");
    }
  }    
}
