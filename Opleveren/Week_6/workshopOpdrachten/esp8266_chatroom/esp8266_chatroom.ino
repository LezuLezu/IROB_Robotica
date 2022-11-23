#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>   // Include the SPIFFS library

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
File fsUploadFile;                                    // a File variable to temporarily store the received file
StaticJsonDocument<256> jsonBuffer;

void handleNotFound();

void setup() {  
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  startWiFi();

  startMDNS();

  startServer();

  SPIFFS.begin();                           // Start the SPI Flash Files System
}

void startWiFi() { // Try to connect to some given access points. Then wait for a connection
  wifiMulti.addAP("SSID", "password");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("SSID", "password");

  Serial.println("Connecting");
  while (wifiMulti.run() != WL_CONNECTED) {  // Wait for the Wi-Fi to connect
    delay(250);
    Serial.print('.');
  }
  Serial.println("\r\n");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());             // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.print(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer
  Serial.println("\r\n");
}

void startMDNS() { // Start the mDNS responder
  const char *mdnsName = "chatroom";

  MDNS.begin(mdnsName);                        // start the multicast domain name server
  Serial.print("mDNS responder started: http://");
  Serial.print(mdnsName);
  Serial.println(".local");
}

void startServer() { // Start a HTTP server with a file read handler and an upload handler
  server.on("/upload", HTTP_POST, handleMessage);
  
  server.onNotFound(handleNotFound);          // if someone requests any other file or page, go to function 'handleNotFound'

  server.begin();                             // start the HTTP server
  Serial.println("HTTP server started.");
}

void handleNotFound() { // if the requested file or page doesn't exist, return a 404 not found error
  if (!handleFileRead(server.uri())) {        // check if the file exists in the flash memory (SPIFFS), if so, send it
    server.send(404, "text/plain", "404: File Not Found");
  }
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
      path += ".gz";                                         // Use the compressed verion
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
  return false;
}

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".csv")) return "text/csv";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void handleMessage(){
  Serial.println("entered handleMessage==================================");

  // Retrieve json from arguments
  String json = server.arg("plain");

  // Create a Json document and buffer eventually turn the json argument
  // into a character array
  DynamicJsonDocument doc(1024);
  char buf[255];
  json.toCharArray(buf, 255);

  // If there is an error exit the function
  auto error = deserializeJson(doc, buf);
  if(error){
    return;
  }  

  // Retrieve information from doc based on key
  String date = doc["date"];
  String clientTime = doc["time"];
  String message = doc["message"];

  Serial.println(message);
  
  File chatMessages = SPIFFS.open("/chat.csv", "a");
  chatMessages.print(date);
  chatMessages.print(",");
  chatMessages.print(clientTime);
  chatMessages.print(",");
  chatMessages.print(message);
  chatMessages.println("");
  chatMessages.close();
}

void loop() {
  server.handleClient();                    // Listen for HTTP requests from clients
}
