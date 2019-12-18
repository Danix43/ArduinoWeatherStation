#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN D3
#define DHTTYPE DHT11

const char* ssid = "Danix43";
const char* password = "danix1923";
const char* host = "danix43.go.ro";
const size_t capacity = JSON_OBJECT_SIZE(8);

DynamicJsonDocument payload(capacity);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("System started!");
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid, password);  
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
  }
  dht.begin();
  Serial.println("System connected");
  Serial.print(WiFi.localIP());
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  payload["name"] = "Adam01_Alpha";
  payload["firstinsert"] = "2019-09-27T15:20:06.308";
  payload["tempCelsius"] = dht.readTemperature();
  payload["tempKelvin"] = dht.readTemperature() + 273.15;
  payload["tempFahrenheit"] = dht.readTemperature(true);
  payload["humidity"] = dht.readHumidity();

  HTTPClient http;
  http.begin("http://danix43.go.ro:8080/api/termometru/2/update");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Content-Length", "500");
  http.addHeader("Connection", "close");
  String data;
  serializeJson(payload, data);
  int httpCode = http.POST(data);
  String response = http.getString();

  Serial.println(httpCode);
  Serial.println(response);

  http.end();
  ESP.deepSleep(60e6);
}
