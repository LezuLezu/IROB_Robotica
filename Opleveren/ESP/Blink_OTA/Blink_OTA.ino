#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>

ESP8266WiFiMulti wifiMulti;

const byte LED = 13;

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');

//  Removed login credentials 

  wifiMulti.addAP("", "");
  wifiMulti.addAP("", "");
  wifiMulti.addAP("", "");

  Serial.println("Connecting....");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED){
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  
  ArduinoOTA.setHostname("ESP8266");
  ArduinoOTA.setPassword("esp8266");

  ArduinoOTA.onStart([](){
    Serial.println("Start");
  });

    
  ArduinoOTA.onEnd([](){
    Serial.println("\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error){
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive failed");
    else if (error == OTA_END_ERROR) Serial.println("End failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA ready");

  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
}

unsigned long previousTime = millis();
const unsigned long interval = 500;

void loop() {
  ArduinoOTA.handle();
  unsigned long diff = millis() - previousTime;
  if (diff > interval){
    digitalWrite(LED, !digitalRead(LED));
    previousTime += diff;
  }
}
