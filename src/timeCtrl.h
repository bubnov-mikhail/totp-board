#pragma once

#include <Arduino.h>
#include <display.h>
#include <EncButton.h>
#include <DS1307RTC.h>

// Дата и время (Rus)               
const char timeCtrlTitle[] = "\xE0\x61\xBF\x61\x20\xB8\x20\xB3\x70\x65\xBC\xC7"; // {0xE0, 0x61, 0xBF, 0x61, 0x20, 0xB8, 0x20, 0xB3, 0x70, 0x65, 0xBC, 0xC7};
const char timeSeparator = '\x3A';
const char dateSeparator = '\x2E';
const char nonSeparator = '\x20';

class TimeCtrl
{
public:
    TimeCtrl(Display *display, Button *button, DS1307RTC *rtc);
    void execute(void);
    const static unsigned short int yearOffset = 1970;

private:
    enum ClockComponent
    {
        HOURS,
        MINUTES,
        SECONDS,
        DAY,
        MONTH,
        YEAR
    };
    Display *_display;
    Button *_button;
    DS1307RTC *_rtc;
    ClockComponent currentComponent;
    short int componentValue = 0;
    unsigned long blinkLastUpdated;
    unsigned long lastButtonPressedTimestamp;
    bool selectComponent = true; // Highligh a selected component
    const static unsigned short int blinkRefreshMilis = 500; // timeout for current time component blinking
    const static unsigned short int returnTimeoutMilis = 10000; // timeout for exiting from timeCtrl if no button pressed/hold
    const static short int hoursPosition = 0;
    const static short int minutesPosition = 3;
    const static short int secondsPosition = 6;
    const static short int dayPosition = 0;
    const static short int monthPosition = 3;
    const static short int yearPosition = 6;
    void drawComponents(void);
    void printComponent(ClockComponent component, short int value, const char separator);
    void printValue(short int position, short int value, const char separator);
    void nextComponent(void);
    void increaseValue(void);
    bool isReachedTimeout(unsigned long timestamp, unsigned long timeout);
};