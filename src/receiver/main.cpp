#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>

// Налаштування OLED-дисплея
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Налаштування WiFi для вебсервера
const char *ssid = "Wokwi-GUEST";
const char *password = "";
WebServer server(80);
String lastData = "{}";

// Ініціалізація HC-12 через Serial2
HardwareSerial HC12Serial(2);

void handleRoot()
{
  server.send(200, "application/json", lastData);
}

void setup()
{
  Serial.begin(115200);
  HC12Serial.begin(9600, SERIAL_8N1, 17, 18); // HC-12 на GPIO17(RX2), GPIO18(TX2)

  // Ініціалізація OLED-дисплея
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED error!");
    for (;;)
      ;
  }
  display.clearDisplay();

  // Підключення до WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("OK");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Налаштування маршруту вебсервера
  server.on("/", handleRoot);
  server.begin();
}

void loop()
{
  server.handleClient();

  if (HC12Serial.available())
  {
    String data = HC12Serial.readStringUntil('\n');
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

      // Вивід на OLED-дисплей
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
    else
    {
      Serial.println("JSON parse error!");
    }
  }
}
