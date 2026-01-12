#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "struct/DHT11.struct.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void initDisplay() {
    Wire.begin(D2, D1);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    return;
    }

}

void DrawInfo(float lightPercent, int soil, StructDHT11 dht11) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  //display.println("Your plant:");

  //display.println("");

  // Light
  display.print("Light: ");
  display.print(lightPercent, 1); // 1 знак после запятой
  display.println("%");

  // Soil
  display.print("Soil: ");
  display.print(soil);
  display.println("%");

  // Humidity
  display.print("Humidity: ");
  display.print(dht11.humidity);
  display.println("%");

  // Temperature
  display.print("Temperature: ");
  display.print(dht11.temperature);
  display.println("C");

  display.println("");

  // WiFi status
  if (WiFi.status() != WL_CONNECTED) {
    display.println("WiFi not connected");
  } else {
    display.println("WiFi connected");
    display.print("Name: ");
    display.println(WiFi.SSID());
  }

  display.display();
}
