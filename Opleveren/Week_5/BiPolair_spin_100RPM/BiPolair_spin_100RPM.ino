#define A1A 8  //orange
#define A1B 9  // blue
#define B1A 4  // yellow
#define B2A 5  // green

#define LDR_PIN A0

#define NUMBER_OF_STEPS_PER_REV 50
#define WAIT_TIME 3

unsigned int RATES;
unsigned int RPM_calc;
unsigned int RPM_count;

int ldr_treshold = 420;

unsigned long minutes = 60000;  // 60000 for 1 minute, 30000 for 30 second
int timedMinutes = 1.5;

void setup() {
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  pinMode(B1A, OUTPUT);
  pinMode(B2A, OUTPUT);

  pinMode(LDR_PIN, INPUT);


  Serial.begin(9600);
  
  attachInterrupt(LDR_PIN, RPM_Count, analogRead(LDR_PIN) >= ldr_treshold);
}
  
void onestep(){
  write(0, 1, 1, 0);
  delay(WAIT_TIME);
  write(0, 1, 0, 1);
  delay(WAIT_TIME);
  write(1, 0, 0, 1);
  delay(WAIT_TIME);
  write(1, 0, 1, 0);
  delay(WAIT_TIME);

}

void write(int a,int b,int c,int d){
  digitalWrite(A1A,a);
  digitalWrite(A1B,b);
  digitalWrite(B1A,c);
  digitalWrite(B2A,d);
}

void loop() {
  RPM_count = 0;
  if(millis() < minutes * timedMinutes){
    int i;
    i=0;
    while(i<NUMBER_OF_STEPS_PER_REV){
      if(analogRead(LDR_PIN) >= 420){
        RPM_Count();
      }
      onestep();
      i++;
    } 
    RATES ++;
//    Serial.print("Full turn : ");
//    Serial.println(RATES);
  }else{
    Serial.print("Rate per minute calculated: ");
    RPM_calc = RATES / timedMinutes;
    Serial.println(RPM_calc);
    Serial.print("Rate per minute measured: ");
    Serial.println(RPM_count / timedMinutes);
    delay(2000);
  }
}

void RPM_Count(){
  RPM_count++;
}
