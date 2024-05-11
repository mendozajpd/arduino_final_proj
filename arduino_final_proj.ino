#include <Arduino_GFX_Library.h>

Arduino_DataBus *bus = new Arduino_HWSPI(8 /* DC */, 9 /* CS */);

Arduino_GFX *gfx = new Arduino_ILI9341(
  bus, 7 /* RST */, 1 /* rotation */, true /* IPS */);

int choice = 0;

int page = 0;
// 0 = MENU
// 1 = GAME


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

// Millis Counters
unsigned long previousMillis = 0;
unsigned long millisLoop = 750;

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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);

  gfx->begin();
  gfx->fillScreen(BLACK);
  gfx->setCursor(titleLine, titleCenter);
  gfx->setTextColor(WHITE);
  gfx->println("MATH MASTERMIND!!");
  gfx->setTextColor(WHITE, BLACK);
}

void loop() {
  // change_choice();
  buttonHandler();
  Serial.println(difficulty);
  switch (page) {
    case 0:
      startPage();
      break;
    case 1:
      mathPage();
      break;
    default:
      break;
  }
  countBlink();
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
void startChoiceHandler() {
  if (buttonPressed1 || buttonPressed2 || buttonPressed3) {
    clearScreen();
    page = 1;
    buttonPressed1 = false;
    buttonPressed2 = false;
    buttonPressed3 = false;
  }
}


void startPage() {
  startChoice_1();
  startChoiceHandler();
}

void startChoice_1() {
  gfx->setCursor(centerSentence, firstLine);
  gfx->setTextColor(blinkState ? BLACK : WHITE, blinkState ? WHITE : BLACK);
  gfx->print("PRESS ANY BUTTON TO START");
  gfx->setTextColor(blinkState ? WHITE : BLACK, blinkState ? BLACK : WHITE);
}

// void startChoice_2() {
//   gfx->setCursor(centerLong, secondLine);
//   if (choice == 2) {
//     gfx->setTextColor(BLACK, WHITE);
//     gfx->print("START AGAIN");
//     gfx->setTextColor(WHITE, BLACK);
//   } else {
//     gfx->setTextColor(WHITE, BLACK);
//     gfx->print("START AGAIN");
//     gfx->setTextColor(BLACK, WHITE);
//   }
// }

// void startChoice_3() {
//   gfx->setCursor(centerLong, thirdLine);
//   if (choice == 3) {
//     gfx->setTextColor(BLACK, WHITE);
//     gfx->print("START ????");
//     gfx->setTextColor(WHITE, BLACK);
//   } else {
//     gfx->setTextColor(WHITE, BLACK);
//     gfx->print("START ????");
//     gfx->setTextColor(BLACK, WHITE);
//   }
// }


// MATH GAME

void mathChoiceHandler() {
  if (buttonPressed1 || buttonPressed2 || buttonPressed3) {
    difficulty++;
    buttonPressed1 = false;
    buttonPressed2 = false;
    buttonPressed3 = false;
    clearScreen();
  }
}

void mathPage() {
  if (!finishedLoading) {
    mathTitleSetter();
    finishedLoading = true;
  }
  mathChoiceHandler();
}

void mathTitleSetter() {
  gfx->setCursor(titleLine, titleCenter);
  gfx->setTextColor(WHITE);
  gfx->print(" LEVEL ");
  gfx->print(difficulty);
  gfx->setTextColor(WHITE, BLACK);
  delay(500);
}

void mathProblemCreator() {
  gfx->setCursor(centerSentence, firstLine);
  gfx->setTextColor(blinkState ? BLACK : WHITE, blinkState ? WHITE : BLACK);
  gfx->print("LEVEL 1 PROBLEM");
  gfx->setTextColor(blinkState ? WHITE : BLACK, blinkState ? BLACK : WHITE);
}

void change_choice() {
  if (choice + 1 == 4) {
    choice = 1;
  } else {
    choice++;
  }
  delay(1000);
}


void countBlink() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= millisLoop) {
    previousMillis = currentMillis;
    blinkState = !blinkState;
  }
}

void clearScreen() {
  gfx->fillScreen(BLACK);
  finishedLoading = false;
}