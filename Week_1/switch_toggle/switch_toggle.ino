const int LED = 12;
const int BUTTON = 11;

int prestate = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!digitalRead(BUTTON)){
    if(prestate == 0){
      digitalWrite(LED, HIGH);
      prestate = 1;
    }
    else if(prestate == 1){
      digitalWrite(LED, LOW);
      prestate = 0;
    }
  }

 
}
