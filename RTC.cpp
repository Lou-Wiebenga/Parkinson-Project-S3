#include "RTC.h"

RTC_PCF8563 rtc;

void RTC_init (){
    if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC is not initialized, set time.");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    rtc.start();
}

void RTC_setTimeAuto(){
    delay(2000);
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void RTC_setTimeManual(uint16_t Year, uint8_t Month, uint8_t Day, uint8_t Hour, uint8_t Minute, uint8_t Second){
    delay(2000);
    rtc.adjust(DateTime(Year, Month, Day, Hour, Minute, Second));
}

uint16_t RTC_readTime (unit_t time){

    DateTime now = rtc.now();

    switch (time){
        case 0:
            return now.year();
        break;
        case 1:
            return now.month();
        break;
        case 2:
            return now.day();
        break;
        case 3:
            return now.hour();
        break;
        case 4:
            return now.minute();
        break;
        case 5:
            return now.second();
        break;
    }
    return 0;
}

char* RTC_getHhMmString() {		//NOT TESTED
    DateTime now = rtc.now();

    String displayTime = "";
    displayTime += now.hour();
    displayTime += ":";
    displayTime += now.minute();

    char timeFormat[] = "hh:mm";
    char* timeToPrint = now.toString(timeFormat);

    return timeToPrint;
}