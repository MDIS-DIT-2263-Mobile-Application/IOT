#include "WiFi.h";

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#define ssid "Wokwi-GUEST"
#define password ""

#define telegram_token "REPLACE_WITH_TELEGRAM_BOT_ID"
#define CHAT_ID "REPLACE_WITH_YOUR_CHAT_ID"

WiFiClientSecure client;
UniversalTelegramBot bot(telegram_token, client);

const int LED_PIN = 15;     // Connected to esp:15
const int BUTTON_PIN = 13;  // Connected to esp:13

// Variables to track the states
int ledState = LOW;         // Current state of the LED
int lastButtonState = HIGH; // Previous reading from the input pin

// Debounce settings
unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;    

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to internet!");

  // Telegram requires SSL/TLS. 
  // For simulation purposes, we skip certificate validation to save memory.
  client.setInsecure(); 

  // Send a test message on boot
 

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, ledState);
  
  // Set up the button pin with internal pull-up resistor
  // The circuit connects the pin to GND when pressed, so it defaults to HIGH
  pinMode(BUTTON_PIN, INPUT_PULLUP);

   Serial.println("Sending message to Telegram...");
  if (bot.sendMessage(CHAT_ID, "led light monitoring start", "")) {
    Serial.println("Message sent successfully!");
  } else {
    Serial.println("Failed to send message.");
  }
}

void loop() {

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
      
      // Send Telegram notification for turning ON
      Serial.println("Sending Telegram: Light turned ON...");
      bot.sendMessage(CHAT_ID, "💡 <b>Light status changed:</b> The LED has been turned <b>ON</b>.", "HTML");
      
      delay(200); // Short safety pause to prevent double-triggering
    } 
    else if (reading == LOW && ledState == HIGH) {
      ledState = LOW;
      digitalWrite(LED_PIN, ledState);
      
      // Send Telegram notification for turning OFF
      Serial.println("Sending Telegram: Light turned OFF...");
      bot.sendMessage(CHAT_ID, "🌑 <b>Light status changed:</b> The LED has been turned <b>OFF</b>.", "HTML");
      
      delay(200); // Short safety pause to prevent double-triggering
    }
  }

  // Save the reading for the next loop iteration
  lastButtonState = reading;


  
}
