//
// Created by annza on 10/7/2024.
//

#include "smartSleep.h"

smartSleep::smartSleep() : lcd(SS_LCD_ADDRESS), leds(SS_LED_COUNT, LED_DATA_PIN, NEO_GRB + NEO_KHZ800) {

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

void smartSleep::lampSleepSequence(int minutes) {
    bool fadeIn = true;
    bool seqFinished = false;
    unsigned long currMillis = 0;
    unsigned long prevMillis = 0;
    unsigned long delayMs = (minutes * 60 * 1000) / 510;
    uint8_t pBrightness = 0; //pixel brightness

    leds.clear();

    while (!seqFinished) {
      currMillis = millis();
      if ((currMillis - prevMillis) >= delayMs) {
        prevMillis = currMillis;
        lampColor(leds.Color(pBrightness, 0, 0));
        if (fadeIn) {pBrightness++;} else if (!fadeIn) {pBrightness--;}
        if (pBrightness == 255) {fadeIn = false;} else if (pBrightness == 0) {seqFinished = true;}
      }
    }
}

void smartSleep::lampWakeSequence(int minutes) {
    bool fadeIn = true;
    bool seqFinished = false;
    unsigned long currMillis = 0;
    unsigned long prevMillis = 0;
    unsigned long delayMs = (minutes * 60 * 1000) / 510;
    uint8_t pBrightness = 0; //pixel brightness

    leds.clear();

    while (!seqFinished) {
        currMillis = millis();
        if ((currMillis - prevMillis) >= delayMs) {
            prevMillis = currMillis;
            lampColor(leds.Color(0, 0, pBrightness));
            if (fadeIn) {pBrightness++;} else if (!fadeIn) {pBrightness--;}
            if (pBrightness == 255) {fadeIn = false;} else if (pBrightness == 0) {seqFinished = true;}
        }
    }
}

void smartSleep::lampColor(uint32_t color) {
    for (int i=0; i < SS_LED_COUNT; i++) {
      leds.setPixelColor(i, color);
      leds.show();
    }
}
