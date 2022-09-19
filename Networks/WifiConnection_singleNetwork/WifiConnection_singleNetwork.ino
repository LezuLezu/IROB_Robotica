#include <ESP8266WiFi.h>

const char* ssid = "Ziggo8652001";
const char* password = "rkc38xkmzZyt";

void setup(){
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');


  WiFi.begin(ssid, password);
  Serial.println("Connecting to ");
  Serial.println(ssid); 
  Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(++i);
    Serial.print('  ');
  }

  Serial.println('\n');
  Serial.println("Connection Established!");
  Serial.print("IP adress: \t");
  Serial.println(WiFi.localIP());
}

void loop(){}