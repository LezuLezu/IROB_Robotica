#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>

ESP8266WiFiMulti wifiMulti;

WiFiUDP UDP;

IPAddress timeServerIP;

const char* NTPServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48;
byte NTPBuffer[NTP_PACKET_SIZE];

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println('\r\n');

  startWifi();

  startUDP();

  if (!WiFi.hostByName(NTPServerName, timeServerIP)){
    Serial.println("DNS lookup faild, rebooting...");
    Serial.flush();
    ESP.reset();
  }

  Serial.print("Time server IP: \t");
  Serial.println(timeServerIP);

  Serial.println("\r\nSending NTP request ...");
  sendNTPpacket(timeServerIP);
}

unsigned long intervalNTP = 6000;
unsigned long prevNTP = 0;
unsigned long lastNTPResponse = millis();
uint32_t timeUNIX = 0;

unsigned long prevActualTime = 0;

void loop() {
  unsigned long currentMillis = millis();

  if ((currentMillis - prevNTP) > intervalNTP){
    prevNTP = currentMillis;
    Serial.println("\r\nSending NTP request....");
    sendNTPpacket(timeServerIP);
  }

  uint32_t time = getTime();

  if (time){
    timeUNIX = time;
    Serial.print("NTP response:\t");
    Serial.println(timeUNIX);
    lastNTPResponse = currentMillis;
  } else if ((currentMillis - lastNTPResponse) > 3600000){
    Serial.println("More than 1 hour since last NTP response, rebooting...");
    Serial.flush();
    ESP.reset();
  }

  uint32_t actualTime = timeUNIX + (currentMillis - lastNTPResponse) / 1000;
  if (actualTime != prevActualTime && timeUNIX != 0){
    prevActualTime = actualTime;
    Serial.printf("\rUTC time:\t%d:%d:%d ", getHours(actualTime), getMinutes(actualTime), getSeconds(actualTime));
  }
}

void startWifi(){
  wifiMulti.addAP("Ziggo8652001", "rkc38xkmzZyt");
  wifiMulti.addAP("TP-LINK_0768_5G", " 77276995");
  wifiMulti.addAP("TP-LINK_0768", "77276995");

  Serial.println("Connecting.....");
  while (wifiMulti.run() != WL_CONNECTED){
    delay(250);
    Serial.print('.');
  }

  Serial.println('\r\n');

  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());

  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  Serial.println("\r\n");
}

void startUDP(){
  Serial.println("Starting UPD");
  
  UDP.begin(123);

  Serial.print("Local port:\t");
  Serial.println(UDP.localPort());

  Serial.println();
}

uint32_t getTime(){
  if (UDP.parsePacket() == 0){
    return 0;
  }
  UDP.read(NTPBuffer, NTP_PACKET_SIZE);
  uint32_t NTPTime = (NTPBuffer[40]) << 24 | (NTPBuffer[42]) << 16 | (NTPBuffer[42]) << 8 | NTPBuffer[43];
  const uint32_t seventyYears = 2208988800UL;
  uint32_t UNIXTime = NTPTime - seventyYears;
  return UNIXTime;
}

void sendNTPpacket(IPAddress& address){
  memset(NTPBuffer, 0, NTP_PACKET_SIZE);
  NTPBuffer[0] = 0b11100011;
  UDP.beginPacket(address, 123);
  UDP.write(NTPBuffer, NTP_PACKET_SIZE);
  UDP.endPacket();
}

inline int getSeconds(uint32_t UNIXTime){
  return UNIXTime % 60;
}

inline int getMinutes(uint32_t UNIXTime){
  return UNIXTime / 60 % 60;
}

inline int getHours(uint32_t UNIXTime){
  return UNIXTime / 3600 % 24;
}




















