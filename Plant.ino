#include "struct/DHT11.struct.h"
#include "struct/Setting.struct.h"
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>
#include <ESP8266WiFiMulti.h>

const char rootCACert[] = R"EOF(-----BEGIN CERTIFICATE-----
MIIFMDCCBBigAwIBAgIRAPLH5EidQWf2DSajly90piEwDQYJKoZIhvcNAQELBQAw
OzELMAkGA1UEBhMCVVMxHjAcBgNVBAoTFUdvb2dsZSBUcnVzdCBTZXJ2aWNlczEM
MAoGA1UEAxMDV1IxMB4XDTI1MTIyNjE3NDQwMVoXDTI2MDMyNjE3NDQwMFowFzEV
MBMGA1UEAwwMKi52ZXJjZWwuYXBwMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIB
CgKCAQEAlw3X8DOc/V8iYX4w8nnlSjE42JLOTIRJG2H5lIiFx2Crmq4wC5S7sLUp
JerQEs2jf+LpgpG0B7/8E049CzO1HEXECCJJXNjDP4QkSj5OydBXDtuz8+l20a88
YZbqcQLqkitQZYT6onV+qCJlT48TTnOnKqGfmcRhHumZH/RBN9cRlGDHQ8YvVao0
3QTWPqLrJVPM2JK1Gv5NpVlVmtOoEJFunafEXIfc73auQj/OwWfkllwFbhDXJOqB
u3Wxox4sYcbanewA5QdvMhCuGOxhSRvAUscccJn08O/OI3blp5thcDtUCg5DelGR
kQvVwjYdsP/3hTaFEcBWlK775icD9wIDAQABo4ICUTCCAk0wDgYDVR0PAQH/BAQD
AgWgMBMGA1UdJQQMMAoGCCsGAQUFBwMBMAwGA1UdEwEB/wQCMAAwHQYDVR0OBBYE
FOiv8IL/Ma3ZwiOfwTWAexnmieLTMB8GA1UdIwQYMBaAFGZpSdTeKpyRA8+JDiS4
DjADboguMF4GCCsGAQUFBwEBBFIwUDAnBggrBgEFBQcwAYYbaHR0cDovL28ucGtp
Lmdvb2cvcy93cjEvOHNjMCUGCCsGAQUFBzAChhlodHRwOi8vaS5wa2kuZ29vZy93
cjEuY3J0MCMGA1UdEQQcMBqCDCoudmVyY2VsLmFwcIIKdmVyY2VsLmFwcDATBgNV
HSAEDDAKMAgGBmeBDAECATA2BgNVHR8ELzAtMCugKaAnhiVodHRwOi8vYy5wa2ku
Z29vZy93cjEvZnRLMlVkNThNb1UuY3JsMIIBBAYKKwYBBAHWeQIEAgSB9QSB8gDw
AHYASZybad4dfOz8Nt7Nh2SmuFuvCoeAGdFVUvvp6ynd+MMAAAGbW/mVZQAABAMA
RzBFAiEAs/IDtTcbBn53BJNk+steOLLlJr6EHT+HQ9pA1zhRoJ0CIFLIZCKz5O9P
oIa4BwPIB1HwcjtToeoEdXSNqrz2ytmxAHYAlpdkv1VYl633Q4doNwhCd+nwOtX2
pPM2bkakPw/KqcYAAAGbW/mVigAABAMARzBFAiAfIoVpNDTJlQVoHKA5EFCRY3/c
NBXvISFnS74lrSAG3gIhAPWr2bl/3jojzZ9ehowriGXT07H1+FYPeKx8e/MbNdE3
MA0GCSqGSIb3DQEBCwUAA4IBAQCQbrjVyl/prxnefw1Hw9Zu9EQ46RLLcW6K2R9R
HQLgE+dBBqsgfrSX+nA6Bh9OddWgUWqk68D3jtmUUczJqlDl+YdZDIujXYIHeiwW
OloDX3v0b3EXl58+NPiu3CahvZlfdsXwnLvHzcG8MXPAbkdz/Q2C+icutHXjQpqL
gNH8UvF8GEjXfnDec4jwQhFGSkEfZD66kTNHGrnh5zgR6s/lR5xTdYP/AjomlIvB
ULpG/5476e/jCzAldOm+HfAQOUCGflT+/L4fgLDX9uYqI/6Xg8kJC05Ti6NmNbdA
9C9oCmUrQDyKnq6aYErraR9SS1ebJausOGB4y1aVdCV88VOw
-----END CERTIFICATE-----)EOF";

unsigned long lastSend = 0;
unsigned long lastRead = 0;

const unsigned long SEND_INTERVAL = 5000; // 5 сек
const unsigned long READ_INTERVAL = 1000;  // 1 сек
const char* serverName = "<url>";
WiFiClientSecure client;
WebSocketsClient webSocket;
ESP8266WiFiMulti wifiMulti;

void setup() {
  Serial.begin(115200); 
  configTime(0, 0, "pool.ntp.org"); 
  time_t now = time(nullptr);
  BearSSL::X509List cert(rootCACert);
  client.setTrustAnchors(&cert);
  client.setX509Time(now);
  delay(1000); 
  initFS();
  InitDHT();
  initLight();
  initDisplay();
  initWifi();
  GetPointsWiFi();
}



void loop() {
  unsigned long now = millis();

  // Чтение датчиков каждую секунду
  if (now - lastRead >= READ_INTERVAL) {
    lastRead = now;

    StructDHT11 dht11 = getDHT();
    int soil = getSoil();
    float lightPercent = watcherLight();

    DrawInfo(lightPercent, soil, dht11);

    // Отправка данных раз в 30 секунд
    if (now - lastSend >= SEND_INTERVAL) {
      SendData(lightPercent, soil, dht11);
      lastSend = now;
    }
  }

  yield(); // обязательно для ESP8266
}
