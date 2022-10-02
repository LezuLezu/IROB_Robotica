const int SENSORPIN = A0;
const int LED = 5;
int sensorValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SENSORPIN, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(SENSORPIN);
  Serial.println(sensorValue);
  if(sensorValue == 0){
    digitalWrite(LED, LOW);
  }else if(sensorValue > 0 && sensorValue < 50){
    sensorValue = analogRead(SENSORPIN);
    Serial.println(sensorValue); 
    analogWrite(LED, 25.5);
  }else if(sensorValue > 50 && sensorValue < 100){
    analogWrite(LED, 51);
  }else if(sensorValue > 100 && sensorValue < 200){
      sensorValue = analogRead(SENSORPIN);
      Serial.println(sensorValue);
      analogWrite(LED, 76.5);     
  }else if(sensorValue > 300 && sensorValue < 400){
      sensorValue = analogRead(SENSORPIN);
      Serial.println(sensorValue);  
      analogWrite(LED, 102);      
  }else if(sensorValue > 400 && sensorValue < 500){
      sensorValue = analogRead(SENSORPIN);
      Serial.println(sensorValue); 
      analogWrite(LED, 127.5);       
  }else if(sensorValue > 600 && sensorValue < 700){
    analogWrite(LED, 153);
  }else if(sensorValue > 700 && sensorValue < 800){
    analogWrite(LED, 178.5);    
  }else if(sensorValue > 800 && sensorValue < 900){
    analogWrite(LED, 204);
  }else if(sensorValue > 900 && sensorValue < 1000){
      analogWrite(LED, 229.5);
  }else if(sensorValue >= 1000){
      sensorValue = analogRead(SENSORPIN);
      Serial.println(sensorValue); 
      digitalWrite(LED, HIGH);
  }
}
