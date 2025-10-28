#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <HardwareSerial.h>

// Визначення пінів для датчиків
#define DHTPIN 16
#define LDR_PIN 34
#define DHTTYPE DHT22

// Налаштування OLED-дисплея
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Ініціалізація датчика DHT
DHT dht(DHTPIN, DHTTYPE);

// Ініціалізація HC-12 через Serial2
HardwareSerial HC12Serial(2);

void setup()
{
  Serial.begin(115200);                       // Для налагодження через USB
  HC12Serial.begin(9600, SERIAL_8N1, 17, 18); // HC-12 на GPIO17(RX2), GPIO18(TX2)

  // Ініціалізація DHT
  dht.begin();

  // Ініціалізація OLED-дисплея
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
  // Зчитування даних з датчиків
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int light = analogRead(LDR_PIN);

  // Вивід на OLED-дисплей
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

  // Формуємо JSON-рядок
  String data = "{ \"temp\": " + String(temp, 1) +
                ", \"hum\": " + String(hum, 1) +
                ", \"light\": " + String(light) + " }";

  // Відправка через HC-12
  HC12Serial.print(data);
  HC12Serial.print('\n');

  // Для контролю також у Serial Monitor
  Serial.print("Sent: ");
  Serial.println(data);

  delay(2000);
}
