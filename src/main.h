#pragma once

#include <Arduino.h>
#include <buildTime.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <EncButton.h>
#include <timeCtrl.h>
#include "sha1.h"
#include "TOTP.h"
#include <totpPrivateCode.h> // Read the src/_totpPrivateCode.h file to fix the compile error!

#define PIN_BTN 6

bool isReachedTimeout(unsigned long timestamp, unsigned long timeout);
void displayCode();
void displayCodeName();
void printCode(char* code, bool clear);