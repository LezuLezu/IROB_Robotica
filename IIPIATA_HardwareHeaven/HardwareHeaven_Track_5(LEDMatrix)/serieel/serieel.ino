void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
//  while(Serial.available() > 0){
//    if(Serial.read() == 'a'){
////      doet iets
//    }
//  }
  Serial.println("Hello World of Arduino!");
  delay(1000);
}
