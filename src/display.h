#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>

class Display
{
public:
    Display();
    void print(short int position, short int value, bool withDot);
    void clear(short int position, bool withDot);
    void clearAll();
};