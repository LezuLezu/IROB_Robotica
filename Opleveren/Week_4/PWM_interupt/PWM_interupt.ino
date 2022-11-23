//https://www.robotshop.com/community/forum/t/arduino-101-timers-and-interrupts/13072
const int LED = 4;
const int POTPIN = A0;

byte ledBrightness = 1000;

void setup(){
  pinMode(LED, OUTPUT);
  pinMode(POTPIN, INPUT);
  Serial.begin(9600);


  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 34286;            // preload timer 65536-16MHz/256/2Hz
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER1_OVF_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  TCNT1 = 34286;            // preload timer
//  digitalWrite(LED, digitalRead(LED) ^ 1);
  digitalWrite(LED, HIGH);
  delay(ledBrightness);
  digitalWrite(LED, LOW);
}



void loop() {
  Serial.println(analogRead(POTPIN));
  int ledBrightness = map(analogRead(POTPIN), 1, 2023, 0, 255);
  delay(100);
//  digitalWrite(LED, HIGH);
  
}
