#define A1A 8  //orange
#define A1B 9  // blue
#define B1A 4  // yellow
#define B2A 5  // green

#define NUMBER_OF_STEPS_PER_REV 50
#define WAIT_TIME 10

unsigned int RATES;
unsigned int RPM;

unsigned long minutes = 60000;
int timedMinutes = 3;

void setup() {
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  pinMode(B1A, OUTPUT);
  pinMode(B2A, OUTPUT);

  Serial.begin(9600);
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
    RPM = RATES / timedMinutes;
    Serial.println(RPM);
    delay(2000);
  }

}
