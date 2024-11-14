//
// Created by annza on 10/7/2024.
//

#ifndef SMARTSLEEP_H
#define SMARTSLEEP_H

#include <Adafruit_NeoPixel.h>
#include <I2C_LCD.h>
#include "RTC.h"

#define SS_LCD_ADDRESS 0x27
#define SS_LCD_COLUMNS 16
#define SS_LCD_ROWS 2
#define SS_LED_COUNT 12                 //amount of LEDs on ring
#define LED_DATA_PIN 2                  //pin on ESP32 connected to DIN on LED ring

#define RED 0
#define ORANGE 1
#define YELLOW 2
#define GREEN 3
#define BLUE 4
#define PURPLE 5
#define WHITE 6
#define WARM_WHITE 7

#define LO 55
#define MED 155
#define HI 255

class smartSleep {

public:

    smartSleep(); //class constructor

    I2C_LCD lcd;
    Adafruit_NeoPixel leds;

    enum Colors {				//enum abandoned for now
        _RED,
        _ORANGE,
        _YELLOW,
        _GREEN,
        _BLUE,
        _PURPLE,
        _WHITE,
        _WARM_WHITE
    };

    enum BrightnessLevels {		//enum abandoned for now
        _LO = 55,
        _MED = 155,
        _HI = 255
    };

    /*smartSleep::Colors*/ int currColor; //currently set lamp color
    /*smartSleep::BrightnessLevels*/ uint8_t currBrightnessLvl; //currently set brightness level
    bool lampTurnedOn;

    uint8_t wakeHr; //wake time hour
    uint8_t wakeMin; //wake time minute
    uint8_t sleepHr; //sleep time hour
    uint8_t sleepMin; //sleep time minute

    void init(); //initializes LCD, LEDs and RTC

    //bool buttonPressed(int button); //button debounce function, returns true if button pressed, else false
    void lcdPrint(const char* text);
    void lcdPrintCenter(uint8_t row, const char* text); //prints text in the center of row
    void lcdClear(); //clears text on LCD
    void lcdSetCursor(uint8_t col, uint8_t row); //sets lcd cursor
    void lcdPrintTime(uint8_t row, uint8_t hour, uint8_t mins); //prints hour and min in "hh:mm" format in center of row on LCD
    void lcdPrintCurrTime(uint8_t row); //prints current time in "hh:mm" format in center of row on LCD

    void lampFadeIn (int mins, uint8_t maxBrightness, /*smartSleep::Colors*/ int color); //fades LEDs in
    void lampFadeOut (int mins, uint8_t maxBrightness, /*smartSleep::Colors*/ int color); //fades LEDs out
    void lampSleepSequence(int onTimeMins, uint8_t maxBrightness); //fades in red LEDs, stays on for onTimeMins minutes, fades out
    void lampWakeSequence(int onTimeMins, uint8_t maxBrightness); //fades in blue LEDs, stays on for onTimeMins minutes, fades out

    void lampOn(/*smartSleep::Colors*/ int c, uint8_t brightness); //turns on LED ring with color c and set brightness
    void lampOff(); //turns LED ring off

    bool isLampOn(); //returns true if LEDs are on, else returns false

    //void timeSync(); //RTC sync: reads PC time and inputs it into RTC
    uint8_t getCurrHr(); //reads current hour from RTC
    uint8_t getCurrMins(); //reads current minutes from RTC
    void setWakeTime(uint8_t hr, uint8_t mins); //sets wake sequence trigger time
    void setSleepTime(uint8_t hr, uint8_t mins); //sets sleep sequence trigger time

    void setCurrColor(/*smartSleep::Colors*/ int c); //sets lamp color in idle mode
    void setCurrBrightnessLvl(/*smartSleep::BrightnessLevels*/ uint8_t b); //sets lamp brightness in idle mode
    /*smartSleep::Colors*/ int getCurrColor(); //get currently set lamp color
    /*smartSleep::BrightnessLevels*/ uint8_t getCurrBrightnessLvl(); //get currently set lamp brightness

//private:
};



#endif //SMARTSLEEP_H
