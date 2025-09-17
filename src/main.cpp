#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

//WiFi імя 
const char *ssid = "YOUR_WIFI_SSID";
//WiFi пароль
const char *password = "YOUR_WIFI_PASSWORD";

// API ключ з ThingSpeak
String apiKey = "YOUR_API_KEY";

const char *server = "http://api.thingspeak.com/update";
int ldrPin = 34;

void setup()
{
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  Serial.print("Підключення до WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi підключено!");
}

void loop()
{
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int light = analogRead(ldrPin);
  if (isnan(t) || isnan(h))
  {
    Serial.println("Помилка зчитування з DHT22!");
    return;
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    String url = server;
    url += "?api_key=" + apiKey;
    url += "&field1=" + String(t);
    url += "&field2=" + String(h);
    url += "&field3=" + String(light);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0)
    {
      Serial.println("Дані відправлені на ThingSpeak!");
    }
    else
    {
      Serial.println("Помилка відправки!");
    }
    http.end();
  }
  delay(20000); // затримка 20 сек (обмеження ThingSpeak)
}