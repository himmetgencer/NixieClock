#include "mbed.h"
#include "PinDefinitions.h"
#include "nixie.h"
#include "PinDetect.h"

DigitalOut status(statusLed);

NIXIE digit1(DIGIT1_PINA, DIGIT1_PINB, DIGIT1_PINC, DIGIT1_PIND);
NIXIE digit2(DIGIT2_PINA, DIGIT2_PINB, DIGIT2_PINC, DIGIT2_PIND);
NIXIE digit3(DIGIT3_PINA, DIGIT3_PINB, DIGIT3_PINC, DIGIT3_PIND);
NIXIE digit4(DIGIT4_PINA, DIGIT4_PINB, DIGIT4_PINC, DIGIT4_PIND);

PinDetect hourButton(HOUR_BUTTON_PIN, PullDown);
PinDetect minuteButton(MINUTE_BUTTON_PIN, PullDown);

bool hourButtonState = false;
bool hourButtonHeldState = false;
bool minuteButtonState = false;
bool minuteButtonHeldState = false;

void hourButtonPressed(void){
    hourButtonState = true;
}

void hourButtonPressedHeld(void){
    hourButtonHeldState = true;    
}

void hourButtonReleased(void){    
    hourButtonHeldState = false;    
}

void minuteButtonPressed(void){
    minuteButtonState = true;    
}

void minuteButtonPressedHeld(void){
    minuteButtonHeldState = true;    
}

void minuteButtonReleased(void){    
    minuteButtonHeldState = false;        
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
    Clock_Digit digit;
    
	set_time(648810000);  // Set RTC time 24.07.2020 09:00:00

    hourButton.attach_asserted(&hourButtonPressed);
    hourButton.attach_asserted_held(&hourButtonPressedHeld); // Defaults to 1 second
    hourButton.attach_deasserted(&hourButtonReleased);

    minuteButton.attach_asserted(&minuteButtonPressed);
    minuteButton.attach_asserted_held(&minuteButtonPressedHeld); // Defaults to 1 second
    minuteButton.attach_deasserted(&minuteButtonReleased);

    hourButton.setSampleFrequency(); //Defaults to 20ms
    minuteButton.setSampleFrequency(); //Defaults to 20ms
	
    while (true) {

        NIXIE::timeToDigits(digit);
        digit1.setDigit(digit.hour_digit1);
        digit2.setDigit(digit.hour_digit2);
        digit3.setDigit(digit.minute_digit1);
        digit4.setDigit(digit.minute_digit2);

        if(hourButtonState || hourButtonHeldState){
            hourButtonState = false;
            incrementHour();
        }

        if(minuteButtonState || minuteButtonHeldState){
            minuteButtonState = false;
            incrementMinute();
        }

        status = !status;
        ThisThread::sleep_for(250);
	
    }
}