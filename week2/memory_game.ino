// =============================================
// Simon Says - Week 2 Programming Challenge
// ESP32-S2
// =============================================

// ---------- LEDs ----------
#define RED_LED     5
#define GREEN_LED   6
#define BLUE_LED    7

// ---------- Buzzer ----------
#define BUZZER      8

// ---------- Buttons ----------
#define RED_BTN     15
#define GREEN_BTN   16
#define BLUE_BTN    17
#define START_BTN   18

// ---------- Game ----------
int sequence[3];

void setup()
{
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(RED_BTN, INPUT_PULLUP);
  pinMode(GREEN_BTN, INPUT_PULLUP);
  pinMode(BLUE_BTN, INPUT_PULLUP);
  pinMode(START_BTN, INPUT_PULLUP);

  randomSeed(micros());

  allOff();

  Serial.println("==============================");
  Serial.println("SIMON SAYS");
  Serial.println("==============================");
  Serial.println("Press START");
}

void loop()
{
  if (digitalRead(START_BTN) == LOW)
  {
    while (digitalRead(START_BTN) == LOW);

    delay(200);

    startGame();
  }
}

void startGame()
{
  Serial.println();
  Serial.println("Game Started");
  gameStart();
  generateSequence();

  Serial.print("Sequence : ");

  for (int i = 0; i < 3; i++)
  {
    Serial.print(sequence[i]);
    Serial.print(" ");
  }

  Serial.println();

  delay(1000);

  showSequence();

  if (checkSequence())
  {
    Serial.println("Correct!");
    victory();
  }
  else
  {
    Serial.println("Wrong!");
    gameOver();
  }

  Serial.println();
  Serial.println("Press START to play again");
}

// =============================================

void generateSequence()
{
  for (int i = 0; i < 3; i++)
  {
    sequence[i] = random(3);
  }
}

// =============================================

void showSequence()
{
  allOff();

  delay(500);

  for (int i = 0; i < 3; i++)
  {
    showColour(sequence[i]);

    tone(BUZZER, 1000, 150);

    delay(600);

    allOff();

    delay(300);
  }
}

// =============================================

bool checkSequence()
{
  for (int i = 0; i < 3; i++)
  {
    int answer = waitButton();

    showColour(answer);

    tone(BUZZER, 1200, 100);

    delay(250);

    allOff();

    if (answer != sequence[i])
    {
      return false;
    }
  }

  return true;
}

// =============================================

int waitButton()
{
  while (true)
  {
    if (digitalRead(RED_BTN) == LOW)
    {
      while (digitalRead(RED_BTN) == LOW);
      delay(20);
      return 0;
    }

    if (digitalRead(GREEN_BTN) == LOW)
    {
      while (digitalRead(GREEN_BTN) == LOW);
      delay(20);
      return 1;
    }

    if (digitalRead(BLUE_BTN) == LOW)
    {
      while (digitalRead(BLUE_BTN) == LOW);
      delay(20);
      return 2;
    }
  }
}

// =============================================

void showColour(int colour)
{
  allOff();

  switch (colour)
  {
    case 0:
      digitalWrite(RED_LED, HIGH);
      break;

    case 1:
      digitalWrite(GREEN_LED, HIGH);
      break;

    case 2:
      digitalWrite(BLUE_LED, HIGH);
      break;
  }
}



// =============================================

void allOff()
{
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
}

void victory()
{
  int notes[] = {
    523, 659, 784, 1047,
    784, 1047, 1319
  };

  int duration[] = {
    120,120,120,250,
    120,250,400
  };

  for(int i=0;i<7;i++)
  {
    tone(BUZZER, notes[i], duration[i]);
    delay(duration[i] + 30);
  }

  noTone(BUZZER);

  // Victory light animation
  for(int i=0;i<4;i++)
  {
    digitalWrite(RED_LED,HIGH);
    delay(70);

    digitalWrite(GREEN_LED,HIGH);
    delay(70);

    digitalWrite(BLUE_LED,HIGH);
    delay(70);

    allOff();
    delay(70);
  }
}

void gameOver()
{
  int notes[] = {
    784, 698, 659, 587,
    523, 494, 392
  };

  int duration[] = {
    120,120,120,120,
    120,120,350
  };

  for(int i=0;i<7;i++)
  {
    tone(BUZZER, notes[i], duration[i]);

    digitalWrite(RED_LED,HIGH);
    delay(duration[i]);

    digitalWrite(RED_LED,LOW);
    delay(20);
  }

  noTone(BUZZER);
}


void gameStart()
{
  int notes[] = {
    523,659,784
  };

  for(int i=0;i<3;i++)
  {
    tone(BUZZER, notes[i],120);
    delay(170);
  }

  noTone(BUZZER);
}
