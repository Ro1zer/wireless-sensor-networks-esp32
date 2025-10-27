#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
const char *ssid = "Wokwi-GUEST";
const char *password = "";
WebServer server(80);
String lastData = "{}";
void handleRoot()
{
  server.send(200, "application/json", lastData);
}
void setup()
{
  Serial.begin(115200);
  Serial2.begin(9600); // HC-12
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED error!");
    for (;;)
      ;
  }
  display.clearDisplay();
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("OK");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.begin();
}
void loop()
{
  server.handleClient();
  if (Serial2.available())
  {
    String data = Serial2.readStringUntil('\n');
    lastData = data;
    // Розбір JSON
    StaticJsonDocument<200> doc;
    DeserializationError err = deserializeJson(doc, data);
    if (!err)
    {
      float temp = doc["temp"];
      float hum = doc["hum"];
      int light = doc["light"];
      // Вивід у Serial Monitor
      Serial.print("Temp: ");
      Serial.print(temp);
      Serial.print("C, Hum: ");
      Serial.print(hum);
      Serial.print("%, Light: ");
      Serial.println(light);
      // Вивід на OLED
      display.clearDisplay();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.print("T: ");
      display.print(temp);
      display.println(" C");
      display.print("H: ");
      display.print(hum);
      display.println(" %");
      display.print("L: ");
      display.println(light);
      display.display();
    }
  }
}