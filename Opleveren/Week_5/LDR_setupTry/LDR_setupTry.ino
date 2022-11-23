#define LED_PIN 2
#define LDR_PIN A0

unsigned int RPM_count;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);

  Serial.begin(9600);
  
  attachInterrupt(LDR_PIN, RPM_Count, RISING);
}

void loop() {
//  Serial.println("No LED");
//  Serial.println(analogRead(LDR_PIN));
//  delay(1500);
//  Serial.println("LED on");
//  digitalWrite(LED_PIN, HIGH);
//  delay(200);
//  Serial.println(analogRead(LDR_PIN));
//  delay(1300);
//  Serial.println("LED off");
//  digitalWrite(LED_PIN, LOW);
//  delay(1500);

  Serial.println(analogRead(LDR_PIN));
  delay(1000);


}

void RPM_Count(){
  RPM_count++;
  Serial.println("Interrupt");
}
