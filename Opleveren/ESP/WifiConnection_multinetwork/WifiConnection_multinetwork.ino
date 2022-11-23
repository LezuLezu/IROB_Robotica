#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;

void setup(){
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');
  //  Removed login credentials 
  wifiMulti.addAP("", "");
  wifiMulti.addAP("", " ");
  wifiMulti.addAP("", "");

  Serial.println("Connecting.....");
  int i = 0;
  while(wifiMulti.run() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }

  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: \t");
  Serial.println(WiFi.localIP());
}

void loop(){}
