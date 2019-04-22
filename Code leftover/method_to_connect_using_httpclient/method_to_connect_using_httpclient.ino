void sendDataToApi() {
  delay(5000);
  
  HTTPClient http;

  http.begin(client, "192.168.0.160", 8080, "/house/termometre/update?id=2");
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.PUT(result);
  String payload = http.getString();
  if (httpCode == HTTP_CODE_OK) {
    Serial.println("Payload send succesfully");
    Serial.print(payload);    
  } if (httpCode == HTTPC_ERROR_CONNECTION_REFUSED) {
    Serial.println("Couldn't establish connection with the server");
    Serial.println(httpCode);
    Serial.print(payload); 
  } else {
    Serial.print("Error code : ");
    Serial.print(httpCode);
    Serial.println(payload); 
  }
  http.end();
