#include "mbed.h"
#include "PinDefinitions.h"
#include "nixie.h"
#include "PinDetect.h"

DigitalOut status(statusLed);

NIXIE digit1(DIGIT1_PINA, DIGIT1_PINB, DIGIT1_PINC, DIGIT1_PIND);
NIXIE digit2(DIGIT2_PINA, DIGIT2_PINB, DIGIT2_PINC, DIGIT2_PIND);
NIXIE digit3(DIGIT3_PINA, DIGIT3_PINB, DIGIT3_PINC, DIGIT3_PIND);
NIXIE digit4(DIGIT4_PINA, DIGIT4_PINB, DIGIT4_PINC, DIGIT4_PIND);

Clock_Digit digit;

PinDetect hourButton(HOUR_BUTTON_PIN);
PinDetect minuteButton(MINUTE_BUTTON_PIN);

bool hourButtonState = false;
bool minuteButtonState = false;

void hourButtonPressed(void){
    hourButtonState = true;
}

void hourButtonReleased(void){
    hourButtonState = false;
}

void minuteButtonPressed(void){
    minuteButtonState = true;
}

void minuteButtonReleased(void){
    minuteButtonState = false;
}

void incrementHour(void){
    time_t now = time(NULL);
    struct tm *now_tm = localtime(&now);

    now_tm->tm_hour = (now_tm->tm_hour +1) % 24; //increment hour
    now_tm->tm_sec = 0;

    set_time(mktime(now_tm));
}

void incrementMinute(void){
    time_t now = time(NULL);
    struct tm *now_tm = localtime(&now);

    now_tm->tm_min = (now_tm->tm_min +1) % 60; //increment minute
    now_tm->tm_sec = 0;

    set_time(mktime(now_tm));
}

int main()
{
	set_time(648810000);  // Set RTC time 24.07.2020 09:00:00
	
    while (true) {

        NIXIE::timeToDigits(digit);
        digit1.setDigit(digit.hour_digit1);
        digit2.setDigit(digit.hour_digit2);
        digit3.setDigit(digit.minute_digit1);
        digit4.setDigit(digit.minute_digit2);

        if(hourButtonState){
            incrementHour();
        }

        if(minuteButtonState){
            incrementMinute();
        }

        status = !status;
        ThisThread::sleep_for(250);
	
    }
}