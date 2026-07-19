#include "secret.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

WiFiClientSecure espClient;
PubSubClient client(espClient);

// MQTT Topics
const char* TOPIC_SET   = "esp32/led/set";
const char* TOPIC_STATE = "esp32/led/state";

// Pins
const int LED_PIN = 15;
const int BUTTON_PIN = 13;

// LED State
bool ledState = LOW;

// Button debounce
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void publishState()
{
  if (ledState)
    client.publish(TOPIC_STATE, "ON", true);
  else
    client.publish(TOPIC_STATE, "OFF", true);

  Serial.print("Published State : ");
  Serial.println(ledState ? "ON" : "OFF");
}

void callback(char* topic, byte* payload, unsigned int length)
{
  String msg = "";

  for (unsigned int i = 0; i < length; i++)
    msg += (char)payload[i];

  msg.trim();

  Serial.print("Received : ");
  Serial.println(msg);

  if (msg == "ON")
  {
    ledState = HIGH;
  }
  else if (msg == "OFF")
  {
    ledState = LOW;
  }
  else if (msg == "TOGGLE")
  {
    ledState = !ledState;
  }
  else
  {
    return;
  }

  digitalWrite(LED_PIN, ledState);

  publishState();
}

void connectWiFi()
{
  Serial.print("Connecting WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
}

void reconnectMQTT()
{
  while (!client.connected())
  {
    Serial.print("Connecting MQTT...");

    if (client.connect("ESP32Client", mqtt_user, mqtt_password))
    {
      Serial.println("Connected");

      client.subscribe(TOPIC_SET);

      publishState();
    }
    else
    {
      Serial.print(" Failed. rc=");
      Serial.print(client.state());
      Serial.println(" Retry in 2 sec");

      delay(2000);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, LOW);

  connectWiFi();

  // Learning only.
  // Replace with CA certificate in production.
  espClient.setInsecure();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  reconnectMQTT();
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWiFi();
  }

  if (!client.connected())
  {
    reconnectMQTT();
  }

  client.loop();


  // Read the current state of the switch
  int reading = digitalRead(BUTTON_PIN);

  // Check if the switch state changed (due to noise or pressing)
  if (reading != lastButtonState) {
    // Reset the debouncing timer
    lastDebounceTime = millis();
  }

  // Check if the signal has been stable longer than the debounce delay
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Check if the physical button state actually shifted
    // LOW means the button is actively pressed down to GND
    if (reading == LOW && ledState == LOW) {
      ledState = HIGH;
      digitalWrite(LED_PIN, ledState);
      publishState();
     
      delay(200); // Short safety pause to prevent double-triggering
    } 
    else if (reading == LOW && ledState == HIGH) {
      ledState = LOW;
      digitalWrite(LED_PIN, ledState);
      publishState();
     
      delay(200); // Short safety pause to prevent double-triggering
    }
  }

  // Save the reading for the next loop iteration
  lastButtonState = reading;


  
}