const int SENSORPIN = A0;
const int LED = 3;
int sensorValue = 0;
const double STAPGROOTTE = 5.0 / 1023;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(SENSORPIN, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(SENSORPIN);
//  Serial.println(sensorValue);  
//  delay(1000);
  if(sensorValue == 0){
    digitalWrite(LED, HIGH);
  }else if(sensorValue > 0 && sensorValue < 200){
      sensorValue = analogRead(SENSORPIN);
      Serial.println(sensorValue);  
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW);
      delay(100);
      Serial.println("0.1sec");  
  }else if(sensorValue > 200 && sensorValue < 400){
      sensorValue = analogRead(SENSORPIN);
      Serial.println(sensorValue);  
      digitalWrite(LED, HIGH);
      delay(300);
      digitalWrite(LED, LOW);
      delay(300);
      Serial.println("0.3sec"); 
  }else if(sensorValue > 400 && sensorValue < 600){
      sensorValue = analogRead(SENSORPIN);
      Serial.println(sensorValue);  
      digitalWrite(LED, HIGH);
      delay(600);
      digitalWrite(LED, LOW);
      delay(600);
      Serial.println("0.6sec"); 
  }else if(sensorValue > 600 && sensorValue < 800){
      sensorValue = analogRead(SENSORPIN);
      Serial.println(sensorValue);  
      digitalWrite(LED, HIGH);
      delay(900);
      digitalWrite(LED, LOW);
      delay(900);
      Serial.println("0.9sec"); 
  }else if(sensorValue > 800 && sensorValue < 1000){
      digitalWrite(LED, HIGH);
      delay(1200);
      digitalWrite(LED, LOW);
      delay(1200);
      Serial.println("1.2sec"); 
  }else if(sensorValue >= 1000){
      sensorValue = analogRead(SENSORPIN);
      Serial.println(sensorValue); 
      digitalWrite(LED, HIGH);
      delay(1500);
      digitalWrite(LED, LOW);
      delay(1500);
      Serial.println("1.5sec"); 
  }
}
