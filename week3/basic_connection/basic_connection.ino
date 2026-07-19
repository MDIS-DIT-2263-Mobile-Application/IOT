#include "secret.h"
#include "WiFi.h"

#include <WiFiClientSecure.h>

bool isConnected = false;

void setup() {
  int tryCount = 0;
  int maxRetries = 20;
  Serial.begin(115200);
  Serial.println("Initializing WiFi...");
  WiFi.mode(WIFI_STA);
  Serial.println("Setup done!");

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED && tryCount < maxRetries) {
    delay(500);
    Serial.print(".");
    tryCount++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to Wi-Fi!");
    isConnected = true;
    Serial.println("\nConnected to internet!");
  } else {
    Serial.println("\nFailed to connect to Wi-Fi.");
  }
}


void loop() {
  
}