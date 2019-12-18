
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define DHTPIN D3
#define DHTTYPE DHT11

const long utcOffsetInSeconds = 7200;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

const char* ssid = "Danix43";
const char* password = "danix1923";
const char* host = "danix43.go.ro";
const char* lanHost = "192.168.0.160";
const size_t capacity = JSON_OBJECT_SIZE(8);

DynamicJsonDocument payload(capacity);
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(9600);
  Serial.println("System started!");
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  }
  dht.begin();
  timeClient.begin();
  Serial.println("System connected");
  Serial.print(WiFi.localIP());
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  delay(5000);
  timeClient.update();
  float temperature = dht.readTemperature();
  float temperatureFahrenheit = dht.readTemperature(true);
  float humidity = dht.readHumidity();

  payload["id"] = 2;
  payload["name"] = "Celsius01";
  payload["firstinsert"] = "22-09-2019";
  payload["lastinsert"] = timeClient.getFormattedTime(); 
  payload["tempCelsius"] = temperature;
  payload["tempKelvin"] = temperature + 273.15;
  payload["tempFahrenheit"] = temperatureFahrenheit;
  payload["humidity"] = humidity;

  Serial.println(timeClient.getFormattedTime());

  if (client.connect(lanHost, 8080)) {
    client.println("POST /api/termometru/update/2 HTTP/1.1");
    client.println("Host: 192.168.0.160:8080");
    client.println("User-Agent: WeatherStation/0.1");
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.println("Content-Length:");
    client.println(measureJson(payload));
    client.println();
    serializeJson(payload, client);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Data send!");
  }
  client.stop();
}
