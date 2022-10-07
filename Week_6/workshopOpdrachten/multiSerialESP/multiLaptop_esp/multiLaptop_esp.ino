#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>   // Include the SPIFFS library

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
File fsUploadFile;                                    // a File variable to temporarily store the received file

void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();

void setup() {  
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');

  startAP();

  startMDNS();

  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server

  SPIFFS.begin();                           // Start the SPI Flash Files System
}

void startAP() {
  const char *ssid = "Chatroom";
  const char *password = "DoeIetsLeuks";
  
  WiFi.softAP(ssid, password);             // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");
  
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer
}

void startMDNS() { // Start the mDNS responder
  const char *mdnsName = "chatroom";

  MDNS.begin(mdnsName);                        // start the multicast domain name server
  Serial.print("mDNS responder started: http://");
  Serial.print(mdnsName);
  Serial.println(".local");
}

void startServer() { // Start a HTTP server with a file read handler and an upload handler
  server.onNotFound(handleNotFound);          // if someone requests any other file or page, go to function 'handleNotFound'
  // and check if the file exists

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

void loop() {
  server.handleClient();
}
