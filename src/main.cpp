#include <Arduino.h>
#include "DHT.h"
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
const int LDR_PIN = 34;
const int RX_PIN = 16; // ESP32 RX2
const int TX_PIN = 17; // ESP32 TX2
unsigned long lastSend = 0;
void setup()
{
  Serial.begin(115200);
  dht.begin();
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("ESP32 -> HC-12: старт передачі даних");
}
void loop()
{
  if (millis() - lastSend > 5000)
  {
    lastSend = millis();
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    int ldr = analogRead(LDR_PIN);
    if (isnan(t) || isnan(h))
    {
      Serial.println("Помилка DHT22!");
      Serial2.println("ERR:DHT");
      return;
    }
    String msg = "T:" + String(t, 1) + " H:" + String(h, 1) + " L:" + String(ldr);
    Serial.println("-> " + msg);
    Serial2.println(msg); // надсилаємо у HC-12
  }
}