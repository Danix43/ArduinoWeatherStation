#include <DHTesp.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "Danix43";
const char* password = "danix1923";
const char* host = "danix43.go.ro";

DynamicJsonDocument payload(512);
DHTesp dht;

void setup() {
  Serial.begin(115200);
  Serial.println("System started!");
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid, password);  
  while(WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(80);
    digitalWrite(LED_BUILTIN, LOW);
  }
  dht.setup(D4, DHTesp::DHT11);
  Serial.println("System connected");
  Serial.print(WiFi.localIP());
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  delay(dht.getMinimumSamplingPeriod());

  double humidity = dht.getHumidity();
  double temperatureCelsius = dht.getTemperature();
  double heatIndexCelsius = dht.computeHeatIndex(temperatureCelsius, humidity, false);
  double temperatureFahrenheit = dht.toFahrenheit(temperatureCelsius);
  double heatIndexFahrenheit = dht.computeHeatIndex(temperatureCelsius, humidity, true);
  double temperatureKelvin = dht.getTemperature() + 273.15;
//  needs more research
  double heatIndexKelvin = heatIndexCelsius + 273.15;

  Serial.println(dht.getStatusString());
  Serial.println("Data to be send: ");
  Serial.println(temperatureCelsius);
  Serial.println(heatIndexCelsius);
  Serial.println(temperatureFahrenheit);
  Serial.println(heatIndexFahrenheit);
  Serial.println(temperatureKelvin);
  Serial.println(heatIndexKelvin);
  Serial.println(humidity);

  payload["name"] = "Adam01";
  payload["location"] = "test";
  payload["firstInsert"] = "2019-11-30T18:08:39.000+0000";
  payload["lastInsert"] = "2019-11-30T18:08:39.000+0000";
  payload["temperatureInCelsius"] = temperatureCelsius;
  payload["heatIndexCelsius"] = heatIndexCelsius;
  payload["temperatureInKelvin"] = temperatureKelvin;
  payload["heatIndexKelvin"] = heatIndexKelvin;
  payload["temperatureInFahrenheit"] = temperatureFahrenheit;
  payload["heatIndexFahrenheit"] = heatIndexFahrenheit;
  payload["humidity"] = humidity;

  WiFiClient client;
  if(client.connect(host, 8080)) {
    client.println("POST /api/termometru/1/update HTTP/1.1");
    client.println("Host: danix43.go.ro");
    client.println("User-Agent: WeatherStation/1.1");
    client.println("Connection: close");
    client.println("Content-Type: application/json;charset=UTF-8");
    client.print("Content-Length: ");
    client.println(measureJson(payload));
    client.println();
    serializeJson(payload, client);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(80);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Data send!");
  }
  serializeJson(payload, Serial);
  client.stop();
  delay(10000);
}
