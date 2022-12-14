#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define ONE_HOUR 3600000UL

ESP8266WebServer server(80);
File fsUploadFile;
ESP8266WiFiMulti wifiMulti;

const char* OTAName = "ESP8266";
const char* OTAPassword = "esp8266";
const char* mdnsName = "esp8266";

WiFiUDP UDP;

IPAddress timeServerIP;
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println('\r\n');

  dht.begin();

  startWiFi();
  startOTA();
  startSPIFFS();
  startMDNS();
  startServer();
  startUDP();
  
  WiFi.hostByName(ntpServerName, timeServerIP);
  Serial.print("Time server IP:\t");
  Serial.println(timeServerIP);
}

const unsigned long intervalNTP = ONE_HOUR;
unsigned long prevNTP = 0;
unsigned long lastNTPResponse = millis();

const unsigned long intervalTemp = 60000;
unsigned long prevTemp = 0;
bool tmpRequested = false;
const unsigned long DS_delay(2000);

uint32_t timeUNIX = 0;

void loop() {

  // Request time from time server every hour
  unsigned long currentMillis = millis();
  if(currentMillis - prevNTP > intervalNTP){
    prevNTP = currentMillis;
    sendNTPpacket(timeServerIP);
  }

// check for response from time server
  uint32_t time = getTime();
  if(time){
    timeUNIX = time;
    Serial.print("NTP response: \t");
    Serial.println(timeUNIX);
    lastNTPResponse = millis();
  } else if ((millis() - lastNTPResponse) > 24UL * ONE_HOUR){
    Serial.println("More tahn 24 hours sinse last ntp response, rebooting...");
    Serial.flush();
    ESP.reset();
  }

    
  if(timeUNIX != 0){
    if((currentMillis - prevTemp) > intervalTemp){

      tmpRequested = true;
      prevTemp = currentMillis;
      Serial.println("Data requested");
    }
    if ((currentMillis - prevTemp) > DS_delay && tmpRequested){
      uint32_t actualTime = timeUNIX + (currentMillis - lastNTPResponse) / 1000;
      tmpRequested = false;
      float hum = dht.readHumidity();
      float temp = dht.readTemperature();
      if (isnan(hum) || isnan(temp)){
        Serial.println(F("Failed to read from DHT sensor"));
        return;
      }
      float hic = dht.computeHeatIndex(temp, hum, false);
      hum = round(hum * 100.0) / 100.0;
      temp = round(temp * 100.0) / 100.0;

      Serial.printf("Appending temperature to file : %lu,", actualTime);
      Serial.print(temp);
      Serial.print(',');
      Serial.println(hum);

      File templog = SPIFFS.open("/temp.csv", "a");
      templog.print(actualTime);
      templog.print(',');
      templog.print(temp);
      templog.print(',');
      templog.print(hum);
      templog.print('\n');
      templog.close();
    }
  } else{
    sendNTPpacket(timeServerIP);
    delay(500);
   }

  server.handleClient();
  ArduinoOTA.handle();
}

void startWiFi() { 
//  Removed login credentials 
  wifiMulti.addAP("", "");  
  wifiMulti.addAP("  ", "");

  Serial.println("Connecting");
  while (wifiMulti.run() != WL_CONNECTED) { 
    delay(250);
    Serial.print('.');
  }
  Serial.println("\r\n");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());      
  Serial.print("IP address:\t");
  Serial.print(WiFi.localIP());    
  Serial.println("\r\n");
}

void startUDP() {
  Serial.println("Starting UDP");
  UDP.begin(123);                     
  Serial.print("Local port:\t");
  Serial.println(UDP.localPort());
}

void startOTA() { 
  ArduinoOTA.setHostname(OTAName);
  ArduinoOTA.setPassword(OTAPassword);

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\r\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA ready\r\n");
}

void startSPIFFS() { 
  SPIFFS.begin();             
  Serial.println("SPIFFS started. Contents:");
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {               
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }
}

void startMDNS() { 
  MDNS.begin(mdnsName);        
  Serial.print("mDNS responder started: http://");
  Serial.print(mdnsName);
  Serial.println(".local");
}

void startServer() { 
  server.on("/edit.html",  HTTP_POST, []() { 
    server.send(200, "text/plain", "");
  }, handleFileUpload);    
  server.onNotFound(handleNotFound);    

  server.begin();           
  Serial.println("HTTP server started.");
}

/*__________________________________________________________SERVER_HANDLERS__________________________________________________________*/

void handleNotFound() { 
  if (!handleFileRead(server.uri())) {     
    server.send(404, "text/plain", "404: File Not Found");
  }
}

bool handleFileRead(String path) { 
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";      
  String contentType = getContentType(path); 
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { 
    if (SPIFFS.exists(pathWithGz))                   
      path += ".gz";                             
    File file = SPIFFS.open(path, "r");                
    size_t sent = server.streamFile(file, contentType);   
    file.close();                                         
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);  
  return false;
}

void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  String path;
  if (upload.status == UPLOAD_FILE_START) {
    path = upload.filename;
    if (!path.startsWith("/")) path = "/" + path;
    if (!path.endsWith(".gz")) {                       
      String pathWithGz = path + ".gz";                 
      if (SPIFFS.exists(pathWithGz))                     
        SPIFFS.remove(pathWithGz);
    }
    Serial.print("handleFileUpload Name: "); Serial.println(path);
    fsUploadFile = SPIFFS.open(path, "w");               
    path = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); 
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {         
      fsUploadFile.close();      
      Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
      server.sendHeader("Location", "/success.html");    
      server.send(303);
    } else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}

/*__________________________________________________________HELPER_FUNCTIONS__________________________________________________________*/

String formatBytes(size_t bytes) { 
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
}

String getContentType(String filename) { 
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

unsigned long getTime() { 
  if (UDP.parsePacket() == 0) { 
    return 0;
  }
  UDP.read(packetBuffer, NTP_PACKET_SIZE); 
  uint32_t NTPTime = (packetBuffer[40] << 24) | (packetBuffer[41] << 16) | (packetBuffer[42] << 8) | packetBuffer[43];
  const uint32_t seventyYears = 2208988800UL;
  uint32_t UNIXTime = NTPTime - seventyYears;
  return UNIXTime;
}


void sendNTPpacket(IPAddress& address) {
  Serial.println("Sending NTP request");
  memset(packetBuffer, 0, NTP_PACKET_SIZE); 
  packetBuffer[0] = 0b11100011; 
 
  UDP.beginPacket(address, 123); 
  UDP.write(packetBuffer, NTP_PACKET_SIZE);
  UDP.endPacket();
}
