
int ledState = 0;
int LED_PIN = 7;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  readSerialPort();
  if(ledState != ""){
    putLedState();
    sendData();
  }
  delay(500);
  
}

void readSerialPort(){  
  if(Serial.available() > 0){
    delay(10);
    while(Serial.available() > 0){
      ledState = Serial.parseInt();
    }
    Serial.flush();
  }
}

void sendData(){
  Serial.print("Arduino turned LED ");
  if(ledState == 1){
    Serial.println("on");
  }else{
    Serial.println("off");
  }
}

void putLedState(){
  digitalWrite(LED_PIN, ledState);  
}
