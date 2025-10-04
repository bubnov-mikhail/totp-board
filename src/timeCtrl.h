#pragma once

#include <Arduino.h>
#include <display.h>
#include <EncButton.h>
#include <DS1307RTC.h>

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
    const static short int minutesPosition = 2;
    const static short int secondsPosition = 4;
    const static short int dayPosition = 0;
    const static short int monthPosition = 2;
    const static short int yearPosition = 4;
    void drawComponents(void);
    void printComponent(ClockComponent component, short int value);
    void printValue(short int position, short int value);
    void nextComponent(void);
    void increaseValue(void);
    bool isReachedTimeout(unsigned long timestamp, unsigned long timeout);
};