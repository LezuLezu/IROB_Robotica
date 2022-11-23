#define A 4
#define B 5
#define C 6
#define D 7

#define NUMBER_OF_STEPS_PER_REV 512
#define WAIT_TIME 0.5

unsigned long minutes = 60000;
int timedMinutes = 3;

unsigned int RATES;

void setup() {
  Serial.begin(9600);
  
//  Define pinmodes
  pinMode(IR_LED,OUTPUT);
  pinMode(IR_REC, INPUT);

  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
}

void onestep(){
  write(1,0,0,0);
  delay(WAIT_TIME);
  write(1,1,0,0);
  delay(WAIT_TIME);
  write(0,1,0,0);
  delay(WAIT_TIME);
  write(0,1,1,0);
  delay(WAIT_TIME);
  write(0,0,1,0);
  delay(WAIT_TIME);
  write(0,0,1,1);
  delay(WAIT_TIME);
  write(0,0,0,1);
  delay(WAIT_TIME);
  write(1,0,0,1);
  delay(WAIT_TIME);
}
void write(int a,int b,int c,int d){
  digitalWrite(A,a);
  digitalWrite(B,b);
  digitalWrite(C,c);
  digitalWrite(D,d);
}

void loop() {
  if(millis() < minutes * timedMinutes){
    int i;
    i=0;
    while(i<NUMBER_OF_STEPS_PER_REV){
    onestep();
    i++;
    } 
    RATES ++;
    Serial.print("Full turn : ");
    Serial.println(RATES);
  }else{
    Serial.print("Rate per minute: ");
    int rpm = RATES / timedMinutes;
    Serial.println(rpm);
    delay(2000);
  }

}
