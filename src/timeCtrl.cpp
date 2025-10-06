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
    _display->printChars(0, 0, timeCtrlTitle);
    
    drawComponents();
    while (true)
    {
      _button->tick();
      if (_button->release())
      {
        break;
      }
    }

    while (true)
    {
        drawComponents();

        _button->tick();
        if (_button->hold()) {
            blinkLastUpdated = millis() - blinkRefreshMilis;
            lastButtonPressedTimestamp = millis();
            nextComponent();
            continue;
        }

        if (_button->click()) {
            blinkLastUpdated = millis() - blinkRefreshMilis;
            lastButtonPressedTimestamp = millis();
            increaseValue();
            continue;
        }

        if (isReachedTimeout(lastButtonPressedTimestamp, returnTimeoutMilis)) {
            _display->clearAll();
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
        printComponent(HOURS, tm.Hour, timeSeparator);
        printComponent(MINUTES, tm.Minute, timeSeparator);
        printComponent(SECONDS, tm.Second, nonSeparator);
        break;
    case DAY:
    case MONTH:
    case YEAR:
        printComponent(DAY, tm.Day, dateSeparator);
        printComponent(MONTH, tm.Month, dateSeparator);
        printComponent(YEAR, yearOffset + tm.Year - 2000, nonSeparator);
        break;
    }

    blinkLastUpdated = millis();
    selectComponent = !selectComponent;
}

void TimeCtrl::printComponent(ClockComponent component, short int value, const char separator)
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
        _display->clear(1, position);
        _display->clear(1, position+1);
    } else {
        printValue(position, value, separator);
    }
}

void TimeCtrl::printValue(short int position, short int value, const char separator)
{
    if (value < 10)
    {
        _display->print(1, position, 0);
        _display->print(1, position+1, value);
    } else {
        _display->print(1, position, value);
    }
    _display->printChar(1, position+2, separator);
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

    tmElements_t tm;
    _rtc->read(tm);
    
    switch (currentComponent)
    {
    case HOURS:
        componentValue = tm.Hour;
        min = 0;
        max = 23;
        break;
    case MINUTES:
        componentValue = tm.Minute;
        min = 0;
        max = 59;
        break;
    case SECONDS:
        componentValue = tm.Second;
        min = 0;
        max = 59;
        break;
    case DAY:
        componentValue = tm.Day;
        min = 1;
        max = 31;
        break;
    case MONTH:
        componentValue = tm.Month;
        min = 1;
        max = 12;
        break;
    case YEAR:
        componentValue = yearOffset + tm.Year;
        min = 2025;
        max = 2050;
        break;
    }

    componentValue++;

    if (componentValue < min)
    {
        componentValue = max;
    }
    if (componentValue > max)
    {
        componentValue = min;
    }

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
