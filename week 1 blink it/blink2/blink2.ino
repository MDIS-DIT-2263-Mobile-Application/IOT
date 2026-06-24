// =========================
// Traffic Light Pins
// =========================
#define RED     13
#define YELLOW  12
#define GREEN   14

// =========================
// Pedestrian Light Pins
// =========================
#define PED_RED     22
#define PED_GREEN   23

// Push button for pedestrian request
#define PED_BUTTON  18

void setup() {

  Serial.begin(115200);

  // Configure traffic lights as outputs
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

  // Configure pedestrian lights as outputs
  pinMode(PED_RED, OUTPUT);
  pinMode(PED_GREEN, OUTPUT);

  // Configure button with internal pullup resistor
  pinMode(PED_BUTTON, INPUT_PULLUP);

  // =========================
  // Initial State
  // Cars can move
  // Pedestrians must stop
  // =========================
  digitalWrite(GREEN, HIGH);

  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);

  digitalWrite(PED_RED, HIGH);
  digitalWrite(PED_GREEN, LOW);

  Serial.println("System Ready");
}

void loop() {

  // ==========================================
  // Wait until pedestrian button is pressed
  // INPUT_PULLUP means:
  // NOT pressed = HIGH
  // Pressed     = LOW
  // ==========================================
  if (digitalRead(PED_BUTTON) == LOW) {

    Serial.println("Pedestrian Request");

    // Small debounce delay
    delay(200);

    // ==========================================
    // Step 1
    // Stop traffic
    // ==========================================

    digitalWrite(GREEN, LOW);

    digitalWrite(YELLOW, HIGH);
    Serial.println("Traffic Yellow");
    delay(1000);

    digitalWrite(YELLOW, LOW);

    digitalWrite(RED, HIGH);
    Serial.println("Traffic Red");

    // ==========================================
    // Step 2
    // Allow pedestrians to cross
    // ==========================================

    digitalWrite(PED_RED, LOW);
    digitalWrite(PED_GREEN, HIGH);

    Serial.println("Pedestrian Green");
    delay(5000);

    // ==========================================
    // Step 3
    // Stop pedestrians
    // ==========================================

    digitalWrite(PED_GREEN, LOW);
    digitalWrite(PED_RED, HIGH);

    Serial.println("Pedestrian Red");
    delay(1000);

    // ==========================================
    // Step 4
    // Resume traffic
    // ==========================================

    digitalWrite(RED, LOW);

    digitalWrite(GREEN, HIGH);

    Serial.println("Traffic Green");

    // Wait for button release
    while (digitalRead(PED_BUTTON) == LOW) {
      delay(10);
    }
  }
}