#include "EspMQTTClient.h"
#include "DHT.h"

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
EspMQTTClient client(
   "Danix43",
   "danix1923",
   "192.168.0.160",
   "Danix43",
   "danix1923",
   "WeatherStation");


void setup() {
  Serial.begin(9600);
  Serial.println("System started!");

  dht.begin();
  client.enableDebuggingMessages();
}

void onConnectionEstablished() {
  char buffer[10];
  char* temperaturePayload = dtrostrf(dht.readTemperature(), 1, 4, buffer);
  client.publish("topic/temperature", temperaturePayload);
//  client.publish("topic/temperatureF", dht.readTemperature(true));
//  client.publish("topic/humidity", dht.readHumidity());
  Serial.println("Data send");
  Serial.println(dht.readTemperature());
  Serial.println(dht.readTemperature(true));
  Serial.println(dht.readHumidity());
}
  
void loop() {
  client.loop();
}
