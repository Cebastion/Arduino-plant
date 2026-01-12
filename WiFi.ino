#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include "struct/DHT11.struct.h"
#include "struct/Setting.struct.h"

char ssid[] = "";         
char pass[] = "";     
const uint32_t connectTimeoutMs = 5000;
Setting setting = ReadFile();



void initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();      
  delay(100);

  wifiMulti.addAP(ssid, pass);

  for(int i = 0; i < setting.length; i++) {
    wifiMulti.addAP(setting.WiFis[i].ssid.c_str(), setting.WiFis[i].password.c_str());
  }

  if (wifiMulti.run(connectTimeoutMs) == WL_CONNECTED) {
    Serial.println("WiFi connected");    
  } else {
    Serial.println("WiFi not connected");
  }
}
