#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer server(80);

File fsUploadFile;

String getContentType(String filename);
bool handleFileRead(String path);
void handleFileUpload();

void setup(void){
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP("Ziggo8652001", "rkc38xkmzZyt");
  wifiMulti.addAP("TP-LINK_0768_5G", "77276995");
  wifiMulti.addAP("TP-LINK_0768", "77276995");

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

  SPIFFS.begin();

  server.on("/upload", HTTP_GET, [](){
    if (!handleFileRead("/upload.html"))
      server.send(404, "text/plain", "404: NOT FOUND");      
  });

  server.on("/upload", HTTP_POST, [](){
    server.send(200);
  },
  handleFileUpload
  );
  server.onNotFound([](){
    if (!handleFileRead(server.uri()))
        server.send(404, "text/plain", "404: Not Found");
  });


  server.begin();
  Serial.println("HTTP server started");

}

void loop(void){
  server.handleClient();
}

String getContentType(String filename){
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "plain/text";
}

bool handleFileRead(String path){
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path = ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if (SPIFFS.exists(pathWithGz)){
      path += ".gz";
    } 
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;
}

void handleFileUpload(){
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    if( !filename.startsWith("/")) filename = "/" + filename;
    Serial.print("handleFileUpload name: ");
    Serial.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String(); 
  }
  else if (upload.status == UPLOAD_FILE_WRITE){
    if(fsUploadFile){
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  }
  else if(upload.status == UPLOAD_FILE_END){
    if(fsUploadFile){
      fsUploadFile.close();
      Serial.print("handleFileUpload Size: ");
      Serial.println(upload.totalSize);
      server.sendHeader("Location", "/succes.html"); 
      server.send(303)     ;
    } else{
      server.send(500, "text/plain", "500: could not create file");
    }
  }
} 















