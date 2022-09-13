const int LED = 12;
const int BUTTON = 11;

int buttonState = 0;
int prestate = 0;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(BUTTON);
  if(buttonState == HIGH && prestate == 0){
    count++;
    Serial.println(count);
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    prestate = 1;
  }
  else if (buttonState == LOW){
    prestate = 0;
  }
 
}
