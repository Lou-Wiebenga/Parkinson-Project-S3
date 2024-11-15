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
    uint8_t wakeMins; //wake time minutes
    uint8_t sleepHr; //sleep time hour
    uint8_t sleepMins; //sleep time minutes

    void init(); //initializes LCD, LEDs and RTC

    //bool buttonPressed(int button); //button debounce function, returns true if button pressed, else false
    void lcdPrint(const char* text);
    void lcdPrintCenter(uint8_t row, const char* text); //prints text in the center of row
    void lcdClear(); //clears text on LCD
    void lcdSetCursor(uint8_t col, uint8_t row); //sets lcd cursor
    void lcdHome(); //resets cursor position
    void lcdPrintTime(uint8_t row, uint8_t hour, uint8_t mins); //prints hour and min in "hh:mm" format in center of row on LCD
    void lcdPrintCurrTime(uint8_t row); //prints current time in "hh:mm" format in center of row on LCD
	void lcdInputModeOn(); //turns on blinking cursor for user input
    void lcdInputModeOff();//turns off blinking cursor

    void lampFadeIn (int mins, uint8_t maxBrightness, /*smartSleep::Colors*/ int color); //fades LEDs in
    void lampFadeOut (int mins, uint8_t maxBrightness, /*smartSleep::Colors*/ int color); //fades LEDs out
    void lampSleepSequence(int onTimeMins, uint8_t maxBrightness); //fades in red LEDs, stays on for onTimeMins minutes, fades out
    void lampWakeSequence(int onTimeMins, uint8_t maxBrightness); //fades in blue LEDs, stays on for onTimeMins minutes, fades out

    void lampOn(/*smartSleep::Colors*/ int c, uint8_t brightness); //turns on LED ring with color c and set brightness
    void lampOff(); //turns LED ring off

    bool isLampOn(); //returns true if LEDs are on, else returns false

    uint8_t getCurrHr();//returns current hour
    uint8_t getCurrMins();//returns current minutes
    uint8_t getWakeHr(); //returns currently set wake hr
    uint8_t getWakeMins();//returns currently set wake minutes
    uint8_t getSleepHr();//returns currently set sleep hr
    uint8_t getSleepMins();//returns currently set wake minutes

    void setWakeHr(uint8_t hr); //sets wake sequence trigger hour
    void setWakeMins(uint8_t mins);//sets wake sequence trigger minutes
    void setSleepHr(uint8_t hr); //sets sleep sequence trigger hour
    void setSleepMins(uint8_t mins);//sets wake sequence trigger minutes

    void setCurrColor(/*smartSleep::Colors*/ int c); //sets lamp color in idle mode
    void setCurrBrightnessLvl(/*smartSleep::BrightnessLevels*/ uint8_t b); //sets lamp brightness in idle mode
    /*smartSleep::Colors*/ int getCurrColor(); //get currently set lamp color
    /*smartSleep::BrightnessLevels*/ uint8_t getCurrBrightnessLvl(); //get currently set lamp brightness

//private:
};



#endif //SMARTSLEEP_H
