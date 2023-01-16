#include <WiFi.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"

const char* ssid = "ssid";
const char* password = "pass";

String serverName = "http://192.168.1.71/api/light";

const int relay = 23;
 
void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to the WiFi network");

}
 
void loop() {

  if(WiFi.status()== WL_CONNECTED){
      
      HTTPClient client;
      client.begin(serverName);
      int httpCode = client.GET();

      if(httpCode > 0 ){
        
        String payload = client.getString();
        Serial.println("1: " + payload);

        StaticJsonDocument<50> doc;
        deserializeJson(doc,payload);

        String status = doc["status"];
        Serial.println(status);
        if(status == "false"){
          digitalWrite(relay, LOW);
        }else if(status == "true"){
          digitalWrite(relay, HIGH);
        }
     
      }else{
        Serial.println("Error HTTP Request");
      }
      

    
  }
  
  delay(5000);

}
