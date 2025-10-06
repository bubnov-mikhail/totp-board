#pragma once

#include <Arduino.h>
#include "TOTP.h"

struct HmacKey
{
    TOTP* totp;
    const char name[16];
};
