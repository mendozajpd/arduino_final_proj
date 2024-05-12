#include <Arduino_GFX_Library.h>

Arduino_DataBus *bus = new Arduino_HWSPI(8 /* DC */, 9 /* CS */);

Arduino_GFX *gfx = new Arduino_ILI9341(
  bus, 7 /* RST */, 1 /* rotation */, true /* IPS */);

int choice = 0;

int page = 0;
// 0 = MENU
// 1 = GAME
// 2 = GAMEOVER


// Set to 0 when changing, set to 1 after its done
bool finishedLoading = false;

int titleLine = 115;
int titleCenter = 10;

int centerSmall = 150;
int centerLong = 130;
int centerSentence = 90;
int firstLine = 80;
int secondLine = 100;
int thirdLine = 120;
int fourthLine = 140;
int fifthLine = 160;

// Blink
bool blinkState = false;
bool blinkStateFast = false;

// Millis Counters
unsigned long previousMillis = 0;
unsigned long millisLoop = 750;
unsigned long previousMillisFast = 0;
unsigned long millisLoopFast = 100;
unsigned long countDownTime = 0;
unsigned long second = 1000;

// Button
int buttonPin1 = 2;
int buttonPin2 = 3;
int buttonPin3 = 4;
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

int buttonPressed1 = false;
int buttonPressed2 = false;
int buttonPressed3 = false;

// GAME STATES
int difficulty = 1;
int timeLeft;
int beginningTime = 15;
int x;
int y;
int z;

int answer;
int choiceAnswer;
int choiceRandom1;
int choiceRandom2;
int choiceRandom3;

int score;
int previousScore;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  randomSeed(analogRead(0));

  gfx->begin();
  gfx->fillScreen(BLACK);
  gfx->setCursor(titleLine, firstLine);
  gfx->setTextColor(WHITE);
  gfx->println("MATH MASTERMIND!!");
  gfx->setTextColor(WHITE, BLACK);
}

void loop() {
  // change_choice();
  buttonHandler();
  switch (page) {
    case 0:
      startPage();
      break;
    case 1:
      mathPage();
      break;
    case 2:
      gameOverPage();
      break;
    default:
      break;
  }
  countBlink();
  countBlinkFast();
}

void buttonHandler() {
  buttonHandler1();
  buttonHandler2();
  buttonHandler3();
}

void buttonHandler1() {
  buttonState1 = digitalRead(buttonPin1);
  if (buttonState1 > 0 && digitalRead(buttonPin2) == 0 && digitalRead(buttonPin3) == 0) {
    // Serial.println("1");
    buttonPressed1 = true;
  }
}

void buttonHandler2() {
  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 > 0 && digitalRead(buttonPin1) == 0 && digitalRead(buttonPin3) == 0) {
    // Serial.println("2");
    buttonPressed2 = true;
  }
}

void buttonHandler3() {
  buttonState3 = digitalRead(buttonPin3);
  if (buttonState3 > 0 && digitalRead(buttonPin1) == 0 && digitalRead(buttonPin2) == 0) {
    buttonPressed3 = true;
  }
}

// START STATE
void startGameHandler() {
  if (buttonPressed1 || buttonPressed2 || buttonPressed3) {
    clearScreen();
    page = 1;
    timeLeft = beginningTime;
    score = 0;
    difficulty = 1;
    buttonPressed1 = false;
    buttonPressed2 = false;
    buttonPressed3 = false;
  }
}


void startPage() {
  startChoice_1();
  startGameHandler();
}

void startChoice_1() {
  gfx->setCursor(centerSentence, thirdLine);
  gfx->setTextColor(blinkState ? BLACK : WHITE, blinkState ? WHITE : BLACK);
  gfx->print("PRESS ANY BUTTON TO START");
  gfx->setTextColor(blinkState ? WHITE : BLACK, blinkState ? BLACK : WHITE);
}

// MATH GAME

void mathChoiceHandler() {
  mathChoiceAccept();
  mathChoiceUp();
  mathChoiceDown();
}

void mathChoiceAccept() {
  if (buttonPressed2) {
    if (choice == choiceAnswer) {
      difficulty++;
      gfx->setCursor(centerSentence, firstLine - 40);
      gfx->setTextColor(GREEN, BLACK);
      gfx->print("CORRECT!");
      gfx->setCursor(centerSentence, firstLine - 20);
      gfx->print("+5 TIME");
      gfx->setTextColor(WHITE, BLACK);
      timeLeft += 5;
      score = score + difficulty + timeLeft;
      delay(500);
    } else {
      gfx->setCursor(centerSentence, firstLine - 40);
      gfx->setTextColor(RED, BLACK);
      gfx->print("WRONG");
      gfx->setCursor(centerSentence, firstLine - 20);
      gfx->print("-3 TIME");
      gfx->setTextColor(WHITE, BLACK);
      if (timeLeft - 3 <= 0) {
        timeLeft = 0;
        gameOverHandler();
      } else {
        timeLeft -= 3;
      }
      delay(500);
    }
    buttonPressed2 = false;
    clearScreen();
  }
}

void mathChoiceUp() {
  if (buttonPressed1) {
    if (choice - 1 == 0) {
      choice = 3;
    } else {
      choice--;
    }
    delay(100);
    buttonPressed1 = false;
  }
}

void mathChoiceDown() {
  if (buttonPressed3) {
    if (choice + 1 == 4) {
      choice = 1;
    } else {
      choice++;
    }
    delay(100);
    buttonPressed3 = false;
  }
}

void mathPage() {
  if (!finishedLoading) {
    mathTitleSetter();
    mathProblemCreator();
    finishedLoading = true;
  }
  scoreHandler();
  mathChoices();
  mathChoiceHandler();
  countdownHandler();
  if (timeLeft <= 0) {
    gameOverHandler();
  }
}

void gameOverHandler() {
  clearScreen();
  finishedLoading = false;
  page = 2;
}

void mathChoices() {
  mathChoice_1();
  mathChoice_2();
  mathChoice_3();
  timerHandler();
}

void mathTitleSetter() {
  gfx->setCursor(0, titleCenter);
  gfx->setTextColor(WHITE);
  gfx->print(" LEVEL ");
  gfx->print(difficulty);
  gfx->setTextColor(WHITE, BLACK);
  delay(500);
}

void mathProblemCreator() {
  x = random(1, 10 + difficulty);  // Generate a random number between 1 and 10 for x
  y = random(1, 10 + difficulty);  // Generate a random number between 1 and 10 for y
  z;
  String problem;

  // Decide randomly whether the problem will be an addition or subtraction problem
  if (random(0, 2) == 0) {
    z = x + y;
    problem = "x + y = z";
  } else {
    z = x - y;
    problem = "x - y = z";
  }

  // Decide randomly which of the three numbers will be missing
  switch (random(0, 3)) {
    case 0:
      problem.replace("x", "?");
      problem.replace("y", String(y));
      problem.replace("z", String(z));
      answer = x;
      choiceAnswer = 1;
      break;
    case 1:
      problem.replace("x", String(x));
      problem.replace("y", "?");
      problem.replace("z", String(z));
      answer = y;
      choiceAnswer = 2;
      break;
    case 2:
      problem.replace("x", String(x));
      problem.replace("y", String(y));
      problem.replace("z", "?   ");
      choiceAnswer = 3;
      answer = z;
      break;
  }

  choiceRandom1 = random(1, 10) * (random(0, 2) == 0 ? -1 : 1);
  choiceRandom2 = random(1, 10) * (random(0, 2) == 0 ? -1 : 1);
  choiceRandom3 = random(1, 10) * (random(0, 2) == 0 ? -1 : 1);

  gfx->setCursor(centerSentence, firstLine);
  gfx->setTextColor(WHITE, BLACK);
  gfx->print(problem);
}

void mathChoice_1() {
  int displayAnswer = answer;
  if (choiceAnswer != 1) {
    displayAnswer = displayAnswer + choiceRandom1;
  }

  gfx->setCursor(centerSmall, secondLine);
  gfx->setTextColor(choice == 1 ? BLACK : WHITE, choice == 1 ? WHITE : BLACK);
  gfx->print(displayAnswer);
  gfx->setTextColor(WHITE, BLACK);
  gfx->print("   ");
}

void mathChoice_2() {
  int displayAnswer = answer;
  if (choiceAnswer != 2) {
    displayAnswer = displayAnswer + choiceRandom2;
  }

  gfx->setCursor(centerSmall, thirdLine);
  gfx->setTextColor(choice == 2 ? BLACK : WHITE, choice == 2 ? WHITE : BLACK);
  gfx->print(displayAnswer);
  gfx->setTextColor(WHITE, BLACK);
  gfx->print("   ");
}

void mathChoice_3() {
  int displayAnswer = answer;
  if (choiceAnswer != 3) {
    displayAnswer = displayAnswer + choiceRandom3;
  }

  gfx->setCursor(centerSmall, fourthLine);
  gfx->setTextColor(choice == 3 ? BLACK : WHITE, choice == 3 ? WHITE : BLACK);
  gfx->print(displayAnswer);
  gfx->setTextColor(WHITE, BLACK);
  gfx->print("   ");
}

void timerHandler() {
  gfx->setCursor(centerSentence, titleCenter);
  if (timeLeft < 10 && timeLeft >= 5) {
    gfx->setTextColor(WHITE, BLACK);
    gfx->print("TIME LEFT: ");
    gfx->setTextColor(RED, BLACK);
    gfx->print(timeLeft);
    gfx->print(" ");
    gfx->setTextColor(WHITE, BLACK);
  } else if (timeLeft < 5) {
    gfx->setTextColor(WHITE, BLACK);
    gfx->print("TIME LEFT: ");
    gfx->setTextColor(blinkStateFast ? RED : WHITE, BLACK);
    gfx->print(timeLeft);
    gfx->setTextColor(WHITE, BLACK);
    gfx->print(" ");
    gfx->setTextColor(WHITE, BLACK);
  } else {
    gfx->setTextColor(WHITE, BLACK);
    gfx->print("TIME LEFT: ");
    gfx->print(timeLeft);
    gfx->print(" ");
  }
}

void scoreHandler() {
  gfx->setCursor(centerSentence + 150, titleCenter);
  gfx->setTextColor(WHITE, BLACK);
  gfx->print("SCORE: ");
  gfx->print(score);
  gfx->print(" ");
}

// GAME OVER
void gameOverPage() {
  gameOverChoice_1();
  startGameHandler();
}

void gameOverChoice_1() {
  if (!finishedLoading) {
    if (score > previousScore) {
      gfx->setCursor(centerSentence, 10);
      gfx->setTextColor(WHITE, BLACK);
      gfx->print("CONGRATULATIONS!!! ");
      gfx->setCursor(centerSentence, 30);
      gfx->print("NEW HIGH SCORE: ");
      gfx->print(score);
      gfx->setCursor(centerSentence, 50);
      gfx->print("LVL: ");
      gfx->print(difficulty);
      previousScore = score;
    } else {
      gfx->setCursor(centerSentence, 10);
      gfx->setTextColor(WHITE, BLACK);
      gfx->print("HIGH SCORE: ");
      gfx->print(previousScore);
      gfx->setCursor(centerSentence, 30);
      gfx->print("SCORE: ");
      gfx->print(score);
      gfx->setCursor(centerSentence, 50);
      gfx->print("LVL: ");
      gfx->print(difficulty);
    }
    finishedLoading = true;
  }

  gfx->setCursor(centerSentence, thirdLine);
  gfx->setTextColor(blinkState ? BLACK : WHITE, blinkState ? WHITE : BLACK);
  gfx->print("PRESS ANY BUTTON TO PLAY AGAIN");
  gfx->setTextColor(blinkState ? WHITE : BLACK, blinkState ? BLACK : WHITE);
}

void countBlink() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= millisLoop) {
    previousMillis = currentMillis;
    blinkState = !blinkState;
  }
}

void countBlinkFast() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisFast >= millisLoopFast) {
    previousMillisFast = currentMillis;
    blinkStateFast = !blinkStateFast;
  }
}

void countdownHandler() {
  unsigned long currentMillis = millis();
  if (currentMillis - countDownTime >= second) {
    countDownTime = currentMillis;
    if (timeLeft > 0) {
      timeLeft--;
    }
  }
}

void clearScreen() {
  gfx->fillScreen(BLACK);
  choice = 1;
  finishedLoading = false;
}
