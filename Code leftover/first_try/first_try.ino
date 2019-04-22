#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

ESP8266WiFiMulti WiFiMulti;

const size_t capacity =  JSON_OBJECT_SIZE(4);
DynamicJsonDocument doc(capacity);

int sensorPin = A0;

void setup() {
  Serial.begin(9600);
  Serial.println("System Poseidon01 started");
  pinMode(sensorPin, INPUT);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("WIFI SSID", "WIFI PASSWORD");
  Serial.println("Waiting on establishing the wifi connection");
}

void loop() {
  int sensorValue = 1023 - analogRead(sensorPin);
  if (WiFiMulti.run() == WL_CONNECTED) {
    Serial.println("Connected to the wifi network");
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, "API REQUEST")) {
      Serial.println("Sending the http request");
      doc["machinename"] = "Poseidon01";
      doc["machinetype"] = "PlantObserver";
      doc["location"] = "Camera1";
      doc["currenthum"] = sensorValue;
      char* json;
      serializeJson(doc, json);
      int httpCode = http.PUT(json);
      if (httpCode < 0) {
        Serial.println("HTTP request failed");
        Serial.println(httpCode);
      } else {
        Serial.println("HTTP request send successfuly");
        Serial.println("Content : ");
        Serial.print(json);
      }
      http.end();
    }
  }
}
