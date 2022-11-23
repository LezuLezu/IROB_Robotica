const int A1B = 3;
const int A1A = 4;

void setup() {
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
}

void loop() {
//  Clockwise -> to right
  digitalWrite(A1A, LOW);
  digitalWrite(A1B, HIGH);
  delay(1000);

//  counter Clockwise -> to left
  digitalWrite(A1A, HIGH);
  digitalWrite(A1B, LOW);
  delay(1000);
}
