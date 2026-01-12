#include "LittleFS.h"
#include <ArduinoJson.h>
#include "struct/PointWiFi.struct.h"
#include "struct/Setting.struct.h"


void initFS() {
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed!");
    return;
  }

  Serial.println("LittleFS mounted!");
}

Setting ReadFile() {
  Setting result;

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed!");
    return result;
  }

  File f = LittleFS.open("/PointsWiFi.txt", "r");
  if (!f) {
    Serial.println("File not found!");
    return result;
  }

  StaticJsonDocument<1024> doc;
  DeserializationError err = deserializeJson(doc, f);
  f.close();

  if (err) {
    Serial.print("JSON parse error: ");
    Serial.println(err.c_str());
    return result;
  }

  JsonArray array = doc["WiFis"].as<JsonArray>();
  result.length = doc["length"] | 0;

  if (array.isNull()) {
    Serial.println("WiFis array missing");
    return result;
  }

  result.WiFis.reserve(array.size());

  for (JsonObject obj : array) {
    PointWiFi p;
    p.ssid = obj["ssid"].as<String>();
    p.password = obj["password"].as<String>();
    result.WiFis.push_back(p);
  }

  // защита
  if (result.length != result.WiFis.size()) {
    result.length = result.WiFis.size();
  }

  return result;
}

void WriteFile(const String& jsonBody) {
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed!");
    return;
  }

  StaticJsonDocument<1024> doc;
  DeserializationError err = deserializeJson(doc, jsonBody);

  if (err) {
    Serial.print("JSON parse error: ");
    Serial.println(err.c_str());
    return;
  }

  uint8_t length = doc["length"] | 0;
  JsonArray wifiArray = doc["WiFis"].as<JsonArray>();

  if (wifiArray.isNull()) {
    Serial.println("WiFis array missing");
    return;
  }

  // защита от рассинхрона
  if (length != wifiArray.size()) {
    length = wifiArray.size();
  }

  File f = LittleFS.open("/PointsWiFi.txt", "w");
  if (!f) {
    Serial.println("Failed to open file for writing");
    return;
  }

  StaticJsonDocument<1024> out;
  JsonArray outArray = out.createNestedArray("WiFis");

  for (uint8_t i = 0; i < length; i++) {
    JsonObject src = wifiArray[i];
    JsonObject dst = outArray.createNestedObject();

    dst["ssid"] = src["ssid"].as<String>();
    dst["password"] = src["password"].as<String>();
  }

  out["length"] = length;

  serializeJson(out, f);
  f.close();

  Serial.println("WiFi points saved to file");
}

