#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define DHTPIN D1
#define DHTTYPE DHT11

const size_t capacity = JSON_OBJECT_SIZE(8);
DynamicJsonDocument doc(capacity);

String result;
float temp;
float tempK;
float hum;
DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "WIFI SSID";
char pass[] = "WIFI PASSWORD";
const char* host = "192.168.0.160"; // ip to the api
WiFiClient client;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  dht.begin();
  Serial.println("Machine name: Poseidon01");
  Serial.println("Location: testzone");
  Serial.println("Scope: WeatherStation");
  Serial.println();
}

void loop() {
  getSensorData();
  sendDataToApi();
  Serial.println();
}

void getSensorData() {
  delay(200);
  if (isnan(temp) || isnan(tempK) || isnan(hum)) {
    Serial.println("Failed to read from the sensor!");
    return;
  }
  temp = dht.readTemperature();
  tempK = dht.readTemperature(true);
  hum = dht.readHumidity();

  Serial.print("Temperature in Celsius : ");
  Serial.print(temp);
  Serial.println();
  Serial.print("Temperature in Kelvin : ");
  Serial.print(tempK);
  Serial.println();
  Serial.print("Humidity : ");
  Serial.print(hum);
  Serial.println();
}

void sendDataToApi() {
  delay(5000);

  doc["id"] = 1;
  doc["machinename"] = "Poseidon01";
  doc["machinetype"] = "WeatherStation";
  doc["usedsensor"] = "DHT11";
  doc["location"] = "test1";
  doc["temperature"] = temp;
  doc["temperatureinkelvin"] = tempK;
  doc["humidity"] = hum;

   if (client.connect(host, 8080)) {
    client.println("POST /house/termometre/update?id=1 HTTP/1.1");
    client.println("Host: 192.168.0.160:8080");
    client.println("User-Agent: DWS/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.print("Content-Length:");
    client.println(measureJson(doc));
    client.println();
    serializeJson(doc, client);
    Serial.print("Response :");
    Serial.println(client.readString());
  }
  
}
