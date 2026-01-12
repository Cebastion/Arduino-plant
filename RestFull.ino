#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>



HTTPClient http;


void SendData(float lightPercent, int soil, StructDHT11 dht11) {
    if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }

  String url = String(serverName) + "plant";

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<128> doc;
  doc["light"] = lightPercent;
  doc["soil"] = soil;
  doc["temperature"] = dht11.temperature;
  doc["humidity"] = dht11.humidity;

  String body;
  serializeJson(doc, body);

  int httpCode = http.POST(body);

  Serial.print("HTTP code: ");
  Serial.println(httpCode);

  if (httpCode > 0) {
    String response = http.getString();
    Serial.println("Server response:");
    Serial.println(response);
  } else {
    Serial.println("Request failed");
  }

 

  http.end();
}

void GetPointsWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }

    String url = String(serverName) + "setting";

    http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

int httpCode = http.GET();

Serial.print("HTTP code: ");
  Serial.println(httpCode);

  if (httpCode > 0) {
    String response = http.getString();
    Serial.println("Server response:");
    Serial.println(response);
      WriteFile(response);
  } else {
    Serial.println("Request failed");
  }

 

  http.end();
}


