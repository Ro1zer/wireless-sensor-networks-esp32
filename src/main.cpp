#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---- WiFi ----
const char *ssid = "Wokwi-GUEST"; // у wokwi є відкрита WiFi
const char *password = "";        // без пароля
WebServer server(80);

// ---- DHT22 ----
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ---- OLED ----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---- LDR ----
#define LDR_PIN 34

void handleRoot()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int lightValue = analogRead(LDR_PIN);
  String html = "<html><head><meta htp-equiv='refresh' content='5'></head><body>";
  html += "<h2>Sensor Monitoring</h2>";
  html += "Temperature: " + String(temperature) + " &deg;C<br>";
  html += "Humidity: " + String(humidity) + " %<br>";
  html += "Light: " + String(lightValue) + "<br>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup()
{
  Serial.begin(115200);
  dht.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("Помилка OLED!");
    for (;;)
      ;
  }

  // WiFi підключення
  WiFi.begin(ssid, password);
  Serial.print("Підключення до WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("OK");
  Serial.print("IP адреса: ");
  Serial.println(WiFi.localIP());

  // Web server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server запущено");
}

void loop()
{
  server.handleClient();

  // також оновлюємо OLED
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int lightValue = analogRead(LDR_PIN);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");
  display.print("Hum: ");
  display.print(humidity);
  display.println(" %");
  display.print("Light: ");
  display.println(lightValue);
  display.display();
  delay(2000);
}