#include <display.h>

#define CONTRAST_PIN 9
#define BACKLIGHT_PIN 7
#define CONTRAST 110
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

void Display::print(short int position, short int value, bool withDot)
{
    lcd.setCursor(position, 1);
    lcd.print(value, 10);
}

void Display::clear(short int position, bool withDot)
{
    lcd.setCursor(position, 1);
    lcd.print(char(""));
}

void Display::clearAll()
{
    lcd.clear();
}
