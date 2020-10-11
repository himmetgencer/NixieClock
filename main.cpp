#include "mbed.h"

BusOut ports(p11, p12, p13, p14);
DigitalOut statusLed(LED1);

int main()
{
    while (1) {
        for (int i = 0; i < 10; i++) {
            ports = i;
            //statusLed = !statusLed;
            ThisThread::sleep_for(250);
        }

        for (int i = 8; i > 0; i--) {
            ports = i;
            //statusLed = !statusLed;
            ThisThread::sleep_for(250);
        }        
    }
}