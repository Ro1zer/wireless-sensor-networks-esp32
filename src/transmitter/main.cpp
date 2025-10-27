#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#define DHTPIN 15
#define DHTTYPE DHT22
#define LDR_PIN 34
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);
void setup()
{
  Serial.begin(115200); // Debug
  Serial2.begin(9600);  // HC-12 на GPIO16 (RX2), GPIO17 (TX2)
  dht.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED error!");
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}
void loop()
{
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int light = analogRead(LDR_PIN);
  // Вивід на OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("T: ");
  display.print(temp);
  display.println(" C");
  display.print("H: ");
  display.print(hum);
  display.println(" %");
  display.print("L: ");
  display.println(light);
  display.display();
  // Формуємо JSON
  String data = "{ \"temp\": " + String(temp, 1) +
                ", \"hum\": " + String(hum, 1) +
                ", \"light\": " + String(light) + " }\n";
  // Відправка через HC-12
  Serial2.print(data);
  // Для контролю також у Serial Monitor
  Serial.print("Sent: ");
  Serial.println(data);
  delay(2000);
}