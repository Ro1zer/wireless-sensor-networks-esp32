#include <Arduino.h>
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---- DHT22 ----
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ---- OLED ----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---- LDR module ----
#define LDR_PIN 34 // OUT з модуля підключено сюди

void setup()
{
  Serial.begin(115200);
  Serial.println("Запуск системи моніторингу...");
  dht.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("Помилка: OLED SSD1306 не знайдено!");
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("System Init OK");
  display.display();
  delay(2000);
}

void loop()
{
  delay(2000);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int lightValue = analogRead(LDR_PIN); // 0...4095 для ESP32
  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Помилка зчитування з DHT22!");
    return;
  }

  // --- Serial Monitor ---
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C, Hum: ");
  Serial.print(humidity);
  Serial.print(" %, Light: ");
  Serial.println(lightValue);

  // --- OLED Display ---
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");
  if (temperature > 30)
  {
    display.println("HOT!");
  }
  display.print("Hum: ");
  display.print(humidity);
  display.println(" %");
  display.print("Light:");
  display.println(lightValue);
  if (lightValue < 200)
  {
    display.println("DARK!");
  }
  display.display();
}