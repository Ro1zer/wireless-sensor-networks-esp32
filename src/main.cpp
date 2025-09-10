#include <Arduino.h>
#include "DHT.h"

// Налаштування датчика
#define DHTPIN 15     // Пін підключення DATA
#define DHTTYPE DHT22 // Тип датчика

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(115200);
  Serial.println("Ініціалізація DHT22...");
  dht.begin();
}

void loop()
{
  // Затримка між вимірюваннями
  delay(2000);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Перевірка, чи дані зчитані коректно
  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Помилка зчитування з DHT22!");
    return;
  }

  // Вивід даних у звичайному форматі
  Serial.print("Температура: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Вологість: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Попередження
  if (temperature > 30.0)
  {
    Serial.println("⚠ Попередження: Висока температура!");
  }
  if (humidity > 80.0)
  {
    Serial.println("⚠ Попередження: Висока вологість!");
  }

  // Вивід у форматі JSON
  Serial.print("JSON: {\"temp\": ");
  Serial.print(temperature);
  Serial.print(", \"hum\": ");
  Serial.print(humidity);
  Serial.println("}");
}