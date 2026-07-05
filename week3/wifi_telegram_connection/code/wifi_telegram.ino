#include "WiFi.h";

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#define ssid "Wokwi-GUEST"
#define password ""

//search BotFather from telegram and press start, you will get the token
#define telegram_token "YOUR_TELEGRAM_BOT_TOKEN"

//search ID BOT from telegram and press start, you will get your chat id
#define CHAT_ID "YOUR_CHAT_ID"

WiFiClientSecure client;
UniversalTelegramBot bot(telegram_token, client);

int count=0;
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
  Serial.println("Sending message to Telegram...");
  if (bot.sendMessage(CHAT_ID, "Hello from Wokwi ESP32 Simulator!", "")) {
    Serial.println("Message sent successfully!");
  } else {
    Serial.println("Failed to send message.");
  }
}

void loop() {
  String scount = String(count);
   if (bot.sendMessage(CHAT_ID, "test no "+ scount, "")) {
    Serial.println("Message sent successfully!");
    count++;
  } else {
    Serial.println("Failed to send message.");
    Serial.println("retry no: "+ scount);
  }
  delay(5000);
  
}
