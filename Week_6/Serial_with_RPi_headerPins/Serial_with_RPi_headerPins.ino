
int LED_PIN = 7;
int serialInput;

#define TX 1
#define RX 0
#include <SoftwareSerial.h>
SoftwareSerial pi(RX, TX);

void setup() {
  pi.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TX, OUTPUT);
  pinMode(RX, INPUT);
}

void loop() {
  serialInput = readSerial();
  if(serialInput != ""){
    sendToggle(serialInput);
  }  
  delay(500);
}

int readSerial(){
//  serialInput = 0;
  if(pi.available()){
    delay(50);
    while (pi.available() > 0){
      serialInput = pi.readString().toInt();
    }
    pi.flush();
  }
  return serialInput;
}

void sendToggle(int serialInput){
//  Serial.print("Input in sendToggle : ") ;
//  Serial.println(serialInput);
  if(serialInput == 1){
    digitalWrite(LED_PIN, HIGH);
    pi.println("Arduino turned LED on");
    pi.write("Arduino turned LED on");

  }else if(serialInput == 0){
    pi.println("Arduino turned LED off");
    pi.write("Arduino turned LED off");

    digitalWrite(LED_PIN, LOW);
  }
}
