#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SimpleDHT.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DROPLET_WIDTH 14
#define DROPLET_HEIGHT 14

static const unsigned char PROGMEM DROPLET[] = {
 B00000011, B00000000,
 B00000011, B00000000,
 B00000111, B10000000,
 B00001111, B11000000,
 B00001101, B11000000,
 B00011101, B11100000,
 B00111011, B11110000,
 B00111111, B11110000,
 B00111111, B11110000,
 B00111111, B10110000,
 B00011111, B01100000,
 B00011111, B11100000,
 B00001111, B11000000,
 B00000011, B00000000,
};

#define THERMOMETER_WIDTH 14
#define THERMOMETER_HEIGHT 14

static const unsigned char PROGMEM THERMOMETER[] = {
 B10000011, B00100000,
 B00110100, B10011100,
 B01000110, B10010000,
 B01000100, B10011100,
 B01000110, B10010000,
 B00110100, B10010000,
 B00000110, B10000000,
 B00000100, B10000000,
 B00001111, B11000000,
 B00011111, B11100000,
 B00011111, B01100000,
 B00011110, B11100000,
 B00001111, B11000000,
 B00000111, B10000000,
};

const char* ssid = "TP-Link_DD5E";
const char* password = "16335561";

ESP8266WebServer server(80);

byte temperature = 0;
byte humidity = 0;
void getTemperatureAndHumidity();

int pinDHT11 = D6;
SimpleDHT11 dht11;

void setup()
{
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    return;
  }
  display.clearDisplay();
  display.display();
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", getTemperatureAndHumidity);

  server.enableCORS(true);
  server.begin();
}

void getTemperatureAndHumidity()
{
  String json = "{\n";
  json += "\t\"temperature\":\n";
  json += "\t{\n";
  json += "\t\t\"value\": ";
  json += (int)temperature;
  json += ",\n";
  json += "\t\t\"unit\": \"°C\"\n";
  json += "\t},\n";
  json += "\t\"humidity\":\n";
  json += "\t{\n";
  json += "\t\t\"value\": ";
  json += (int)humidity;
  json += ",\n";
  json += "\t\t\"unit\": \"%\"\n";
  json += "\t}\n";
  json += "}\n";
  server.send(200, "application/json", json);
  Serial.print(json);
}

int tick = 0;
void loop()
{
  server.handleClient();
  MDNS.update();

  display.clearDisplay();
  
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(18, 0);
  display.drawBitmap(0, 0, THERMOMETER, THERMOMETER_WIDTH, THERMOMETER_HEIGHT, WHITE);
  display.print((int)temperature);
  display.print((char)247);
  display.print("C");

  display.setCursor(18, 18);
  display.drawBitmap(0, 18, DROPLET, DROPLET_WIDTH, DROPLET_HEIGHT, WHITE);
  display.print((int)humidity);
  display.print("%");

  display.display();

  if(tick % 100 == 0)
  {
    byte data[40] = {0};
  
    if(dht11.read(pinDHT11, &temperature, &humidity, data))
    {
      Serial.println("Read DHT11 failed");
      return;
    }

    Serial.println("update");
  }
  
//  Serial.print("Temperature: ");
//  Serial.println((int)temperature);
//  
//  Serial.print("Humidity: ");
//  Serial.println((int)humidity);

  tick++;
}
