#include <main.h>

Display *display;
TimeCtrl *timeCtrl;
Button *btn;

char lastTotpCode[7];
char emptyTotpCode[7] = {""};
uint8_t currentTotp = 0;

bool doBlink = false;
bool clear = false;
bool sleepMode = false;
bool lcdUpdateRequired = true;

unsigned short int blink10SecondsMilis = 300;
unsigned short int blink5SecondsMilis = 200;
unsigned short int blink3SecondsMilis = 100;
unsigned short int blinkMilis = blink10SecondsMilis;
unsigned long displayCodeLastUpdated;
unsigned short int displayCodeUpdateMilis = 50;
unsigned long blinkLastUpdated;
unsigned long buttonLastClicked;
unsigned short int backlightTimeoutMilis = 20000;

void setup()
{
  if (!RTC.isRunning())
  {
    tmElements_t tm;
    tm.Day = BUILD_DAY;
    tm.Month = BUILD_MONTH;
    tm.Year = BUILD_YEAR - TimeCtrl::yearOffset;
    tm.Hour = BUILD_HOUR;
    tm.Minute = BUILD_MIN;
    tm.Second = BUILD_SEC;

    RTC.write(tm);
  }

  btn = new Button(PIN_BTN, INPUT_PULLUP, LOW);
  display = new Display();
  timeCtrl = new TimeCtrl(display, btn, &RTC);

  display->clearAll();
  blinkLastUpdated = millis() - blinkMilis;
  displayCodeLastUpdated = millis() - displayCodeUpdateMilis;
  buttonLastClicked = millis();
  lcdUpdateRequired = true;

  displayCodeName();
}

void loop()
{
  btn->tick();

  if (btn->click()) {
    buttonLastClicked = millis();
    if (sleepMode) {
      sleepMode = false;
      display->backlight();
    } else {
      currentTotp++;
      if (currentTotp >= totpCodes) {
        currentTotp = 0;
      }
      clear = false;
      lcdUpdateRequired = true;
      displayCodeName();
    }
  }

  if (btn->hold())
  {
    display->backlight();
    timeCtrl->execute();
    buttonLastClicked = millis();
    clear = false;
    lcdUpdateRequired = true;
    strcpy(lastTotpCode, emptyTotpCode);
    displayCodeName();
  }

  if (isReachedTimeout(displayCodeLastUpdated, displayCodeUpdateMilis))
  {
    displayCodeLastUpdated = millis();
    displayCode();
  }

  if (isReachedTimeout(buttonLastClicked, backlightTimeoutMilis))
  {
    sleepMode = true;
    display->noBacklight();
  }
}

bool isReachedTimeout(unsigned long timestamp, unsigned long timeout)
{
  unsigned long m = millis();
  return m < timestamp ? true : m - timestamp > timeout;
}

void displayCode()
{
  long GMT = RTC.get() + SECS_PER_HOUR * timezoneShiftHours;
  char* newCode = hmacKeys[currentTotp].totp->getCode(GMT);
  lcdUpdateRequired = false;
  if(strcmp(lastTotpCode, newCode) != 0) {
    lcdUpdateRequired = true;
    strcpy(lastTotpCode, newCode);
  }

  tmElements_t tm;
  RTC.read(tm);
  doBlink = (tm.Second >= 20 && tm.Second < 30) || tm.Second >= 50;
  unsigned short int blinkMilis = blink10SecondsMilis;
  if ((tm.Second >= 25 && tm.Second < 30) || tm.Second >= 55)
  {
    blinkMilis = blink5SecondsMilis;
  }
  if ((tm.Second >= 27 && tm.Second < 30) || tm.Second >= 57)
  {
    blinkMilis = blink3SecondsMilis;
  }

  if (!doBlink)
  {
    if (lcdUpdateRequired) {
      printCode(lastTotpCode, false);
    }

    return;
  }

  if (!isReachedTimeout(blinkLastUpdated, blinkMilis))
  {
    return;
  }

  printCode(lastTotpCode, clear);

  blinkLastUpdated = millis();
  displayCodeLastUpdated = millis();
  clear = !clear;
}

void displayCodeName()
{
  display->printChars(0, 0, hmacKeys[currentTotp].name);
}

void printCode(char* code, bool clear)
{
  if (clear) {
    for (uint16_t i = 0; i < 16; i++) {
      display->clear(1, i);
    }
    return;
  }

  display->clear(1, 0);
  display->clear(1, 1);
  for (uint16_t i = 0; i < 6; i++) {
    display->printChar(1, i*2+2, code[i]);
    display->clear(1, i*2+3);
  }
  display->clear(1, 14);
  display->clear(1, 15);
}
