#include <DHT.h>
// Pin Definitions based on your Wokwi diagram
#define DHTPIN 10
#define PIRPIN 11
#define LDR_DIGITAL_PIN 13
#define LDR_ANALOG_PIN 14
#define LED_PIN 15

// Sensor Type
#define DHTTYPE DHT22

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println(F("Smart Room Controller Initializing..."));

  // Initialize Pins
  pinMode(PIRPIN, INPUT);
  pinMode(LDR_DIGITAL_PIN, INPUT);
  pinMode(LDR_ANALOG_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements
  delay(2000);

  // Read environment data
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  // Read motion data
  int motionDetected = digitalRead(PIRPIN);

  // Read light data
  int lightThresholdExceeded = digitalRead(LDR_DIGITAL_PIN);
  int lightIntensity = analogRead(LDR_ANALOG_PIN);

  // Check if any reads failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Print results to Serial Monitor
  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.print(F("°C | Humidity: "));
  Serial.print(humidity);
  Serial.println(F("%"));

  Serial.print(F("Motion Status: "));
  if (motionDetected == HIGH) {
    Serial.println(F("⚠️ MOVEMENT DETECTED!"));
  } else {
    Serial.println(F("No movement."));
  }

  Serial.print(F("Light Level (Analog): "));
  Serial.print(lightIntensity);
  Serial.print(F(" | Dark Threshold Reached: "));
  Serial.println(lightThresholdExceeded == HIGH ? "YES" : "NO");
  if(lightThresholdExceeded == HIGH){

  digitalWrite(LED_PIN, HIGH);
  }
  else{
    
  digitalWrite(LED_PIN, LOW);
  }

  // --- Useful Automation Logic ---
  // Example: Turn on the LED if it's dark AND someone is in the room
  // (Note: Depending on your LDR module, logic state for "dark" may vary)
  if (motionDetected == HIGH && lightIntensity < 1500) { 
    digitalWrite(LED_PIN, HIGH);
    Serial.println(F("Action: Turning ON Nightlight (Motion + Dark)"));
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  
  Serial.println(F("--------------------------------------------------"));
}
