#include <Arduino.h>
#include "DHT.h"
#include "struct/DHT11.struct.h"


#define DHTPIN D5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void InitDHT() {
  pinMode(DHTPIN, INPUT_PULLUP);

  dht.begin();
}

StructDHT11 getDHT() {
  StructDHT11 dht11 = {NAN, NAN};

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    dht11.temperature = t;
    dht11.humidity = h;
  }

 /* Serial.print("Влажность: ");
  Serial.print(dht11.humidity);
  Serial.print("% | Температура: ");
  Serial.print(dht11.temperature);
  Serial.println("°C");*/

  return dht11;
}

