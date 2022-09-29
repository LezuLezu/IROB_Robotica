#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <WebSocketsServer.h>

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80);

WebSocketsServer webSocket(81);

File fsUploadFile;

const char *ssid = "ESP8266 Acces Point";
const char *password = "esp8266";

const char *OTAName = "ESP8266";
const char *OTAPassword = "esp8266";

#define LED_RED 15
#define LED_GREEN 12
#define LED_BLUE 13

const char* mdnsName = "esp8266"; 

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  Serial.begin(9600);
  delay(10);
  Serial.println('\r\t');

  startWifi();
  startOTA();
  startSPIFFS();
  startWebSocket();
  startMDNS();
  startServer();
}
bool rainbow = false;

unsigned long prevMillis = millis();
int hue = 0;

void loop() {
  webSocket.loop();
  server.handleClient();
  ArduinoOTA.handle();

  if(rainbow){
    if(millis() > prevMillis + 32){
      if(++hue == 360){
        hue = 0;
      }
      setHue(hue);
      prevMillis = millis();
    }
  }
}

void startWifi(){
  WiFi.softAP(ssid, password);
  Serial.print("Acces Point \"");
  Serial.print(ssid);
  Serial.println("\" started\r\n");

  //  Removed login credentials 
  wifiMulti.addAP("", "");
  wifiMulti.addAP("", " ");
  wifiMulti.addAP("", "");

  Serial.println("Connecting.....");
  while(wifiMulti.run() != WL_CONNECTED && WiFi.softAPgetStationNum() < 1){
    delay(250);
    Serial.print('.');
  }
  Serial.println("\r\n");
  if(WiFi.softAPgetStationNum() == 0){
    Serial.print("Connected to");
    Serial.println(WiFi.SSID());
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
  } else{
    Serial.print("Station connected to ESP8266 AP");
  }
  Serial.println("\r\n");
}

void startOTA(){
  ArduinoOTA.setHostname(OTAName);
  ArduinoOTA.setPassword(OTAPassword);

  ArduinoOTA.onStart([](){
    Serial.println("Start");
    digitalWrite(LED_RED, 0);
    digitalWrite(LED_GREEN, 0);
    digitalWrite(LED_BLUE, 0);
  });

  ArduinoOTA.onEnd([](){
    Serial.println("\\r\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
    Serial.printf("Progress: %%u%%\r", (progress / (total / 100)));
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
  Serial.println("OTA ready \r\n");
}

void startSPIFFS(){
  SPIFFS.begin();
  Serial.println("SPIFFS started. Contents:");
  { 
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()){
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.println("\n");
  }
}

void startWebSocket(){
  webSocket.begin();  
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started");
}

void startMDNS(){
  MDNS.begin(mdnsName);
  Serial.print("mDNS responder startet: http://");
  Serial.print(mdnsName);
  Serial.println(".local");
}

void startServer(){
  server.on("/edit.html", HTTP_POST, [](){
    server.send(200, "text/plain", "");
  }, handleFileUpload);
  
  server.begin();
  Serial.println("HTTP server started");
}

void handleNotFound(){
  if(!handleFileRead(server.uri())){
    server.send(404, "text/plain", "404:NOT FOUND");
  }
}

bool handleFileRead(String path){
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";

  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";

  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz)) path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    Serial.println(String("\tsent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile not found: ") + path);
  return false;
}

void handleFileUpload(){
  HTTPUpload& upload = server.upload();
  String path;

  if(upload.status == UPLOAD_FILE_START){
    path = upload.filename;
    if(!path.startsWith("/")) path = "/" + path;
    if(!path.endsWith(".gz")){
      String pathWithGz = path + ".gz";
      if(SPIFFS.exists(pathWithGz)) SPIFFS.remove(pathWithGz);
    }
    Serial.print("handleFileUpload Name: ");
    Serial.println(path);
    fsUploadFile = SPIFFS.open(path, "w");
    path = String();        
  } else if (upload.status == UPLOAD_FILE_WRITE){
    if(fsUploadFile) fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END){
    if (fsUploadFile){
      fsUploadFile.close();
      Serial.print("handleFileUpload Size: ");
      Serial.println(upload.totalSize);
      server.sendHeader("Location", "/succes.html");
      server.send(300);
    } else{
      server.send(500, "text/plain", "500: could not create file");
    }
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght){
  switch(type){
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected", num);
      break; 

    case WStype_CONNECTED:{
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      rainbow = false;
    }
    break;

    case WStype_TEXT:
      Serial.printf("[%u] get text: %s\n", num, payload);
      if(payload[0] == '#'){
        uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
        int r = (( rgb >> 20) & 0x3FF);
        int g = (( rgb >> 10) & 0x3FF);
        int b = rgb & 0x3FF;

        analogWrite(LED_RED, r);
        analogWrite(LED_GREEN, g);
        analogWrite(LED_BLUE, b);        
        
      } else if (payload [0] == 'R'){
        rainbow = true;
      } else if (payload[0] == 'N'){
        rainbow = false;
      }
      break;     
  }
}

String formatBytes (size_t bytes){
  if (bytes < 1024){
    return String(bytes + "B");
  } else if (bytes < (1024 * 1024)){
    return String(bytes / 1024) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)){
    return String(bytes / 1024 / 1024) + "MB";
  }
}

String getContentType(String filename){
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript" ;
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void setHue(int hue){
  hue %= 360;
  float radH = hue*3.142/180;
  float rf, gf, bf;

  if(hue >= 0 && hue < 120){
    rf = cos(radH * 3 / 4);
    gf = sin(radH * 3 / 4);
    bf = 0; 
  } else if(hue >= 120 && hue < 240){
    radH -= 2.09439;
    gf = cos(radH * 3 / 4);
    bf = sin(radH * 3 / 4);
    rf = 0;    
  } else if(hue >=240 && hue < 360){
    radH -= 4.188787;
    bf = cos(radH * 3 / 4);
    rf = sin(radH * 3 / 4);
    gf = 0;    
  }

  int r = rf * rf * 1023;
  int g = gf * gf * 1023;
  int b = bf * bf * 1023;

  analogWrite(LED_RED, r);
  analogWrite(LED_GREEN, g);
  analogWrite(LED_BLUE, b);
}
