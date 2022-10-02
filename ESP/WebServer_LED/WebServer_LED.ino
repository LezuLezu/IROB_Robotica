#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80);

const int LED = 2;

void handleRoot();
void handleNotFound();
void handleLED();

void setup(void){
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');

  pinMode(LED, OUTPUT);
  //  Removed login credentials 
  wifiMulti.addAP("", "");
  wifiMulti.addAP("", " ");
  wifiMulti.addAP("", "");

  Serial.println("Connecting....");
  int i = 0; 
  while (wifiMulti.run() != WL_CONNECTED){
    delay(250);
    Serial.print('.');
  }

  Serial.println('\n');
  Serial.print("Connected to: \t");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  if(MDNS.begin("esp8266")){
    Serial.println("mDNS responder started");
  } else{
    Serial.println("Error setting up mDNS responder");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/LED", HTTP_POST, handleLED);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

}

void loop(void){
  server.handleClient();
}

void handleRoot(){
  server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\"value=\"Toggle LED\"></form>");
}

void handleLED(){
  digitalWrite(LED, !digitalRead(LED));
  server.sendHeader("location", "/");
  server.send(303);
}



void handleNotFound(){
  server.send(404, "text/plain", "404: Not Found");
}
