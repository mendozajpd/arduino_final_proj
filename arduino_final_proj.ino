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
int firstLine = 80;
int secondLine = 100;
int thirdLine = 120;
int fourthLine = 140;
int fifthLine = 160;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gfx->begin();
  gfx->fillScreen(BLACK);
  gfx->setCursor(titleLine, titleCenter);
  gfx->setTextColor(WHITE);
  gfx->println("MATH MASTERMIND!!");
  gfx->setTextColor(WHITE, BLACK);
}

void loop() {
  change_choice();
  switch (page) {
    case 0:
      startPage();
      break;
    default:
      break;
  }
}

void startPage() {
  startChoice_1();
  startChoice_2();
  startChoice_3();
}

void startChoice_1() {
  gfx->setCursor(centerSmall, firstLine);
  if (choice == 1) {
    gfx->setTextColor(BLACK, WHITE);
    gfx->print("START");
    gfx->setTextColor(WHITE, BLACK);
  } else {
    gfx->setTextColor(WHITE, BLACK);
    gfx->print("START");
    gfx->setTextColor(BLACK, WHITE);
  }
}

void startChoice_2() {
  gfx->setCursor(centerLong, secondLine);
  if (choice == 2) {
    gfx->setTextColor(BLACK, WHITE);
    gfx->print("START AGAIN");
    gfx->setTextColor(WHITE, BLACK);
  } else {
    gfx->setTextColor(WHITE, BLACK);
    gfx->print("START AGAIN");
    gfx->setTextColor(BLACK, WHITE);
  }
}

void startChoice_3() {
  gfx->setCursor(centerLong, thirdLine);
  if (choice == 3) {
    gfx->setTextColor(BLACK, WHITE);
    gfx->print("START ????");
    gfx->setTextColor(WHITE, BLACK);
  } else {
    gfx->setTextColor(WHITE, BLACK);
    gfx->print("START ????");
    gfx->setTextColor(BLACK, WHITE);
  }
}

void change_choice() {
  if (choice + 1 == 4) {
    choice = 1;
  } else {
    choice++;
  }
  delay(1000);
}
