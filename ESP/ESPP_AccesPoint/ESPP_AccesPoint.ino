#include <ESP8266WiFi.h>

const char *ssid = "ESP8266 Acces Point";
const char *password = "password";


void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');

  WiFi.softAP(ssid, password);
  Serial.print("Acces Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
