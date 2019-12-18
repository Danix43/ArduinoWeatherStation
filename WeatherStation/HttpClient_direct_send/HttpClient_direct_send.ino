#include <ArduinoHttpClient.h>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define DHTPIN D1
#define DHTTYPE DHT11

const char* ssid = "Danix43";
const char* password = "danix1923";
const char* host = "danix43.go.ro";
const char* lanHost = "192.168.0.160";
const size_t capacity = JSON_OBJECT_SIZE(8);

DynamicJsonDocument payload(capacity);
DHT dht(DHTPIN, DHTTYPE);
WiFiClient wifi;
HttpClient client = HttpClient(wifi, lanHost, 8080);

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
  Serial.println("System connected");
  Serial.print(WiFi.localIP());
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  float temperature = dht.readTemperature();
  float temperatureFahrenheit = dht.readTemperature(true);
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(temperatureFahrenheit) || isnan(humidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  payload["id"] = 1;
  payload["machineName"] = "Celsius01";
  payload["temperatureInCelsius"] = temperature;
  payload["temperatureInKelvin"] = temperature + 273.15;
  payload["temperatureInFahrenheit"] = temperatureFahrenheit;
  payload["humidity"] = humidity;

  char* contentType = "application/json";
  String data; 
  serializeJson(payload, data);
  client.post("/api/add?id=1", contentType, data); 
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
}
