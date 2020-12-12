#ifndef NIXIE_H_
#define NIXIE_H_

#include "mbed.h"

class NIXIE
{
public:

    typedef struct Clock_Digit
    {
        uint8_t hour_digit1;
        uint8_t hour_digit2;
        uint8_t minute_digit1;
        uint8_t minute_digit2;
    } Clock_Digit;

    static const uint8_t NONE_Value = 10; // 74141 Driver None Value (H L H L)

    NIXIE(PinName PinA, PinName PinB, PinName PinC, PinName PinD);
    void setDigit(uint8_t value);
    static void timeToDigits(Clock_Digit &digit);
    static bool checkNewHour(void);

private:    
    
    BusOut bus;

};

#endif