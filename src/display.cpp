#include <display.h>

#define CONTRAST_PIN 9
#define BACKLIGHT_PIN 7
#define CONTRAST 50
LiquidCrystal lcd(12, 11, 5, 4, 3, 2, BACKLIGHT_PIN, POSITIVE);

Display::Display()
{
    Serial.begin(57600);

    pinMode(CONTRAST_PIN, OUTPUT);
    analogWrite(CONTRAST_PIN, CONTRAST);

    lcd.backlight();

    lcd.begin(16,2);
    lcd.home ();
}

void Display::print(short int line, short int position, short int value)
{
    lcd.setCursor(position, line);
    lcd.print(value, 10);
}

void Display::printChar(short int line, short int position, char value)
{
    lcd.setCursor(position, line);
    lcd.print(value);
}

void Display::printChars(short int line, short int position, const char value[])
{
    lcd.setCursor(position, line);
    lcd.print(value);
}

void Display::clear(short int line, short int position)
{
    lcd.setCursor(position, line);
    lcd.print("\x20");
}

void Display::backlight()
{
    lcd.backlight();
}

void Display::noBacklight()
{
    lcd.noBacklight();
}

void Display::clearAll()
{
    lcd.clear();
}
