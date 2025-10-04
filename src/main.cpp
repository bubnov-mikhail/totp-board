#include <main.h>

Display *display;
TimeCtrl *timeCtrl;
Button *btn;

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
}

void loop()
{
  // demo
  display->print(0, 0, true); // номер кода
  display->print(1, 2, false);
  display->print(2, 3, true);
  display->print(3, 4, false);
  display->print(4, 5, true);
  display->print(5, 6, false);
  display->print(6, 7, true);

  btn->tick();
  if (btn->hold())
  {
    timeCtrl->execute();
  }
}
