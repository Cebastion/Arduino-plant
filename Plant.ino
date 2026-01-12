#include "struct/DHT11.struct.h"
#include "struct/Setting.struct.h"
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>
#include <ESP8266WiFiMulti.h>

const char rootCACert[] = "<ca>";

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
