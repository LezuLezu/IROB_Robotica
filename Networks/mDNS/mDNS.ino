#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>

ESP8266WiFiMulti wifiMulti;

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println('\n'); 

  wifiMulti.addAP("Ziggo8652001", "rkc38xkmzZyt");
  wifiMulti.addAP("TP-LINK_0768_5G", "77276995");
  wifiMulti.addAP("TP-LINK_0768", "77276995");

  Serial.println("Connecting....");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED){
    delay(1000);
    Serial.print(++i);
    Serial.print(' ');
  }

  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("esp8266")){
    Serial.println("Error setting up mDNS responder!");
  }

  Serial.println("mDNS responder started");
}

void loop() {
  // put your main code here, to run repeatedly:

}
