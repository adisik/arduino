#include <ESP8266WiFi.h>
#include "DHT.h"

const char* ssid = "XXX"; // wifi SSID
const char* password = "XXX"; // wifi password

const int channelId = 0; // thingspeak channel
String writeAPIKey = "XXX"; // thingspeak API key
const char* server = "api.thingspeak.com";
const int postingInterval = 30 * 1000;

#define dhtPin 5
#define dhtType DHT11
DHT myDht(dhtPin, dhtType);

WiFiClient client;

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");

  myDht.begin();
}

void loop() {
  if (client.connect(server, 80)) {

    byte temp = myDht.readTemperature();
    if (isnan(temp)) {
      temp = 0;
    }

    String body = "field1=";
           body += String(temp);
    
    Serial.print("temperature: ");
    Serial.println(temp); 

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(body.length());
    client.print("\n\n");
    client.print(body);
    client.print("\n\n");
  }
  client.stop();

  // wait and then post again
  delay(postingInterval);
}
