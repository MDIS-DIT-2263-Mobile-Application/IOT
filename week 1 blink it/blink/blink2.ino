
#define RED    2
#define YELLOW 4
#define GREEN  5

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);

}


// the loop function runs over and over again forever
void loop() {
 digitalWrite(RED, HIGH);
  Serial.println("RED ON");
  delay(500);
  digitalWrite(RED, LOW);

  digitalWrite(YELLOW, HIGH);
  Serial.println("YELLOW ON");
  delay(500);
  digitalWrite(YELLOW, LOW);

  digitalWrite(GREEN, HIGH);
  Serial.println("GREEN ON");
  delay(500);
  digitalWrite(GREEN, LOW);

}
