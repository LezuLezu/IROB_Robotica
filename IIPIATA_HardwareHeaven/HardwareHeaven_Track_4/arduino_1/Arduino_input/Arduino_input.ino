const int LED = 4;
const int BUTTON = 9;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(BUTTON));
  if(!digitalRead(BUTTON)){
      digitalWrite(LED, HIGH);
  }else{
    digitalWrite(LED, LOW);
  }
}
