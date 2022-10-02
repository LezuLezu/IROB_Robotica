#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80);

void handleRoot();
void handleNotFound();
void handleLogin();

void setup(void){
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');
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
  server.on("/login", HTTP_POST, handleLogin);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

}

void loop(void){
  server.handleClient();
}

void handleRoot(){
  server.send(200, "text/html", 
  "<form action=\"/login\" method=\"POST\"><input type=\"text\" name=\"username\" placeholder=\"Username\"></br><input type=\"password\" name=\"password\" placeholder=\"Password\"></br><input type=\"submit\" value=\"Login\"></form><p>Try 'John Doe' and 'password123' ....</p>");
}

void handleLogin(){
  if ( ! server.hasArg("username") || ! server.hasArg("password") ||
  server.arg("username") == NULL || server.arg("password") == NULL){
    server.send(400, "text/plain", "400: Invalid Request");
    return;
  } 
  if (server.arg("username") == "John Doe" && server.arg("password") == "password123"){
    server.send(200, "text/html", "<h1> Welcome, " + server.arg("username") + "!</h1><p> Login successfull </p>");
  } 
  else{
    server.send(401, "text/plain", "401: Unauthorized");
  }
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not Found");
}
