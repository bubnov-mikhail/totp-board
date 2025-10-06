#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

class Display
{
public:
    Display();
    void print(short int line, short int position, short int value);
    void printChar(short int line, short int position, char value);
    void printChars(short int line, short int position, const char value[]);
    void clear(short int line, short int position);
    void backlight();
    void noBacklight();
    void clearAll();
};