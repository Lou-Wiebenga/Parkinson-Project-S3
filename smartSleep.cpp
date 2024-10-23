//
// Created by annza on 10/7/2024.
//

#include "smartSleep.h"

smartSleep::smartSleep() {

}

void smartSleep::init() {
    leds.begin();
    leds.clear();
    //lcd.begin();
}

void smartSleep::timeCalibrate() {
    //DS3231::setHour();
    //DS3231::setMinute();
    //DS3231::setSecond();
}

void smartSleep::lampSleepSequence() {
    leds.clear();
    for(int i=0; i < SS_LED_COUNT; i++) { // For each pixel in strip...
        leds.setPixelColor(i, leds.Color(155, 0, 0));         //  Set pixel's color (in RAM)
        leds.show();                          //  Update strip to match
        delay(50);                           //  Pause for a moment
    }
}

void smartSleep::lampWakeSequence() {
    leds.clear();
    for(int i=0; i < SS_LED_COUNT; i++) { // For each pixel in strip...
        leds.setPixelColor(i, leds.Color(0, 0, 155));         //  Set pixel's color (in RAM)
        leds.show();                          //  Update strip to match
        delay(50);                           //  Pause for a moment
    }
}