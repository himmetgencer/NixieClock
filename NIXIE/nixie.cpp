#include "nixie.h"

NIXIE::NIXIE(PinName PinA, PinName PinB, PinName PinC, PinName PinD)
:bus(PinA, PinB, PinC, PinD)
{
    setDigit(NONE_Value);
}

void NIXIE::setDigit(uint8_t value)
{
    bus = static_cast<int>(value);
}

void NIXIE::timeToDigits(Clock_Digit &digit)
{
    time_t now;
    struct tm *now_tm;
    now = time(NULL);
    now_tm = localtime(&now);

    digit.hour_digit1 = static_cast<uint8_t>(now_tm->tm_hour / 10);
    digit.hour_digit2 = static_cast<uint8_t>(now_tm->tm_hour % 10);

    digit.minute_digit1 = static_cast<uint8_t>(now_tm->tm_min / 10);
    digit.minute_digit2 = static_cast<uint8_t>(now_tm->tm_min % 10);    
}