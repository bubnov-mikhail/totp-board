#include <timeCtrl.h>

TimeCtrl::TimeCtrl(Display *display, Button *button, DS1307RTC *rtc)
{
    _display = display;
    _button = button;
    _rtc = rtc;
}

void TimeCtrl::execute(void)
{
    blinkLastUpdated = millis() - blinkRefreshMilis;
    lastButtonPressedTimestamp = millis();
    currentComponent = HOURS;
    _display->clearAll();

    while (true)
    {
        drawComponents();

        _button->tick();
        if (_button->releaseHold()) {
            nextComponent();
            continue;
        }

        if (_button->click()) {
            increaseValue();
            continue;
        }

        if (isReachedTimeout(lastButtonPressedTimestamp, returnTimeoutMilis)) {
            return;
        }
    }
}

void TimeCtrl::drawComponents(void)
{
    if (!isReachedTimeout(blinkLastUpdated, blinkRefreshMilis))
    {
        return;
    }

    tmElements_t tm;
    _rtc->read(tm);

    switch (currentComponent)
    {
    case HOURS:
    case MINUTES:
    case SECONDS:
        printComponent(HOURS, tm.Hour);
        printComponent(MINUTES, tm.Minute);
        printComponent(SECONDS, tm.Second);
        break;
    case DAY:
    case MONTH:
    case YEAR:
        printComponent(HOURS, tm.Day);
        printComponent(MINUTES, tm.Month);
        printComponent(SECONDS, yearOffset + tm.Year - 2000); // We have only 2 digits on the display, so let's displaying last decades.
        break;
    }

    blinkLastUpdated = millis();
    selectComponent = !selectComponent;
}

void TimeCtrl::printComponent(ClockComponent component, short int value)
{
    short int position;
    switch (component)
    {
    case HOURS:
        position = hoursPosition;
        break;
    case MINUTES:
        position = minutesPosition;
        break;
    case SECONDS:
        position = secondsPosition;
        break;
    case DAY:
        position = dayPosition;
        break;
    case MONTH:
        position = monthPosition;
        break;
    case YEAR:
        position = yearPosition;
        break;
    }

    if (component == currentComponent && selectComponent) {
        _display->clear(position, false);
        _display->clear(position+1, false);
    } else {
        printValue(position, value);
    }
}

void TimeCtrl::printValue(short int position, short int value)
{
    if (value < 10)
    {
        _display->print(position, 0, false);
        _display->print(position+1, value, true);
    } else {
        _display->print(position, value / 10, false);
        _display->print(position+1, value - value / 10 * 10, true);
    }
}

void TimeCtrl::nextComponent(void)
{
    switch (currentComponent)
    {
    case HOURS:
        currentComponent = MINUTES;
        break;
    case MINUTES:
        currentComponent = SECONDS;
        break;
    case SECONDS:
        currentComponent = DAY;
        break;
    case DAY:
        currentComponent = MONTH;
        break;
    case MONTH:
        currentComponent = YEAR;
        break;
    case YEAR:
        currentComponent = HOURS;
        break;
    }

    blinkLastUpdated = millis() - blinkRefreshMilis;
}

void TimeCtrl::increaseValue(void)
{
    short int min, max;
    componentValue++;
    switch (currentComponent)
    {
    case HOURS:
        min = 0;
        max = 23;
        break;
    case MINUTES:
        min = 0;
        max = 59;
        break;
    case SECONDS:
        min = 0;
        max = 59;
        break;
    case DAY:
        min = 1;
        max = 31;
        break;
    case MONTH:
        min = 1;
        max = 12;
        break;
    case YEAR:
        min = 2025;
        max = 2050;
        break;
    }

    if (componentValue < min)
    {
        componentValue = max;
    }
    if (componentValue > max)
    {
        componentValue = min;
    }

    tmElements_t tm;
    _rtc->read(tm);

    switch (currentComponent)
    {
    case HOURS:
        tm.Hour = componentValue;
        break;
    case MINUTES:
        tm.Minute = componentValue;
        break;
    case SECONDS:
        tm.Second = componentValue;
        break;
    case DAY:
        tm.Day = componentValue;
        break;
    case MONTH:
        tm.Month = componentValue;
        break;
    case YEAR:
        tm.Year = componentValue - yearOffset;
        break;
    }

    _rtc->write(tm);
}

bool TimeCtrl::isReachedTimeout(unsigned long timestamp, unsigned long timeout)
{
    unsigned long m = millis();
    return m < timestamp ? true : m - timestamp > timeout;
}
