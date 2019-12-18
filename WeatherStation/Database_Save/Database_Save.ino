#include "DHT.h"
#include <ESP8266WiFi.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT sensor(DHTPIN, DHTTYPE);

const size_t capacity = JSON_OBJECT_SIZE(8);
DynamicJsonDocument doc(capacity);

const char* ssid = "Danix43";
const char* password = "danix1923";

void setup() {
  Serial.begin(9600);
  Serial.println("Weather station started");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting..");
  }
  sensor.begin();
}

void loop() {
  WiFiClient client;
  doc["id"] = 0;
  doc["machineName"] = "Celsius01";
  doc["dateOfRegistering"];
  doc["lastUpdate"];
  doc["temperatureInCelsius"] = sensor.
  if (!client.connect("danix43.go.ro", 8080)) {
    Serial.println("Can't connect to the remote");
    delay(60000);
    return;
  }
  if (client.connected()) {
        
  }
}
