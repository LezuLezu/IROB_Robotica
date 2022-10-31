
int LED_PIN = 7;
int serialInput;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
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
  if(Serial.available()){
    delay(50);
    while (Serial.available() > 0){
      serialInput = Serial.readString().toInt();
    }
    Serial.flush();
  }
//  Serial.println(serialInput);
  return serialInput;
}

void sendToggle(int serialInput){
//  Serial.print("Input in sendToggle : ") ;
//  Serial.println(serialInput);
  if(serialInput == 1){
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Arduino turned LED on");
  }else if(serialInput == 0){
    Serial.println("Arduino turned LED off");
    digitalWrite(LED_PIN, LOW);
  }
//  else{
//    Serial.print("Something went wrong in comunication");
//  }
}
