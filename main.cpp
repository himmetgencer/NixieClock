#include "mbed.h"
#include "PinDefinitions.h"
#include "nixie.h"

DigitalOut status(statusLed);

NIXIE digit1(DIGIT1_PINA, DIGIT1_PINB, DIGIT1_PINC, DIGIT1_PIND);
NIXIE digit2(DIGIT2_PINA, DIGIT2_PINB, DIGIT2_PINC, DIGIT2_PIND);
NIXIE digit3(DIGIT3_PINA, DIGIT3_PINB, DIGIT3_PINC, DIGIT3_PIND);
NIXIE digit4(DIGIT4_PINA, DIGIT4_PINB, DIGIT4_PINC, DIGIT4_PIND);

Clock_Digit digit;

int main()
{
	set_time(648810000);  // Set RTC time 24.07.2020 09:00:00
	
    while (true) {

        NIXIE::timeToDigits(digit);
        digit1.setDigit(digit.hour_digit1);
        digit2.setDigit(digit.hour_digit2);
        digit3.setDigit(digit.minute_digit1);
        digit4.setDigit(digit.minute_digit2);
        status = !status;
        ThisThread::sleep_for(250);
	
    }
}