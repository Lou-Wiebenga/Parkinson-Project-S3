//
// Created by annza on 10/7/2024.
//

#ifndef SMARTSLEEP_H
#define SMARTSLEEP_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
//#include <I2C_LCD.h>
#include <DFRobotDFPlayerMini.h>
#include "RTC.h"

#define SS_LCD_ADDRESS 0x3C //0x27 - old address for LCD
#define SS_LCD_COLUMNS 16
#define SS_LCD_ROWS 2
#define SCREEN_WIDTH 128                // OLED display width, in pixels
#define SCREEN_HEIGHT 64                // OLED display height, in pixels
#define SS_LED_COUNT 12                 //amount of LEDs on ring
#define LED_DATA_PIN 2                  //pin on ESP32 connected to DIN on LED ring

#define speakersSerial Serial1

#define RED 0
#define ORANGE 1
#define YELLOW 2
#define GREEN 3
#define BLUE 4
#define PURPLE 5
#define WHITE 6
#define WARM_WHITE 7
#define COLORS_AMOUNT 8

#define LO 55
#define MED 155
#define HI 255

#define FADE_MINS 0.5                 //for how many mins red/blue light fades in/out
#define RB_SEQ_MINS 2                //how many minutes the sleep/wake sequence lasts

#define MAX_VOLUME 30				//maximum volume allowed by DFRobot MP3 volume() function

#define MENU_AMOUNT 4				 //amount of menus on the menus screen
#define SOUND_FILES_AMOUNT 4		 //amount of sound files loaded onto SD card

enum Menus {
  M_CALIB_TIME,
  M_SLEEP_WAKE_TIME,
  M_SOUND,
  M_LIGHT
};

class smartSleep {

public:

    smartSleep(); //class constructor

    Adafruit_SSD1306 lcd;
    //I2C_LCD lcd;
    Adafruit_NeoPixel leds;
    DFRobotDFPlayerMini speakers;

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
    char* colors[COLORS_AMOUNT] = {"Red", "Orange", "Yellow", "Green", "Blue", "Purple", "White", "Warm white"};
    /*smartSleep::BrightnessLevels*/ uint8_t currBrightnessLvl; //currently set brightness level
    bool lampTurnedOn;

    //Variables needed for
	unsigned long prevMillisFade = 0;
	unsigned long fadeOutPrevMillis = 0;
	unsigned long fadeDelayMs = FADE_MINS * 60 * 1000 / HI; //delay between brightness ++/-- when fading in/out

	uint8_t pBrightness = 0; //current brightness in fade sequence
    bool doFadeOut = false; //is it time for red/blue light to fade out?

    int selectedMenu = 0; //the menu selected in the menus screen
    char* menus[MENU_AMOUNT] = {"Time calibration", "Sleep/wake time", "Sound", "Light"};

    int wakeSound = 0;
    int sleepSound = 0;
    char* sounds[SOUND_FILES_AMOUNT] = {"None", "White noise", "Brown noise", "Alarm"};
    int soundVolume = 10;

    uint8_t initHr; //initial hour
    uint8_t initMins; //initial minutes
    uint8_t wakeHr; //wake time hour
    uint8_t wakeMins; //wake time minutes
    uint8_t sleepHr; //sleep time hour
    uint8_t sleepMins; //sleep time minutes

    void init(); //initializes LCD, LEDs and RTC

    //bool buttonPressed(int button); //button debounce function, returns true if button pressed, else false
    //void lcdPrint(const char* text);
    template <typename T>
    void lcdPrint(T text){
      //lcd.println(text);
      lcd.print(text);
      //lcd.display();
      //lcd.print(text);
    }

    void lcdShow() {lcd.display();}
    void lcdPrintCenter(uint8_t row, const char* text); //prints text in the center of row
    void lcdClear(); //clears text on LCD
    void lcdSetTextSize(uint8_t size); //set lcd text size
    void lcdSetCursor(uint8_t col, uint8_t row); //sets lcd cursor
    void lcdHome(); //resets cursor position
    void lcdPrintTime(uint8_t hour, uint8_t mins); //prints hour and min in "hh:mm" format in center of row on LCD
    void lcdPrintCurrTime(); //prints current time in "hh:mm" format in center of row on LCD

    void showMenusScreen(); //shows screen with menu selection
	void showSoundScreen(); //shows screen with sound settings (sleep/wake, volume)
    void showLightScreen(); //shows screen with light settings (color, brightness)

    void lampOn(/*smartSleep::Colors*/ int c, uint8_t brightness); //turns on LED ring with color c and set brightness
    void lampOff(); //turns LED ring off

    void lampFadeIn (int mins, uint8_t maxBrightness, /*smartSleep::Colors*/ int color); //fades LEDs in
    void lampFadeOut (int mins, uint8_t maxBrightness, /*smartSleep::Colors*/ int color); //fades LEDs out
    int lampSleepSequence(); //(int fadeMins, int onTimeMins, uint8_t maxBrightness) //fades in red LEDs, stays on for onTimeMins minutes, fades out
    int lampWakeSequence(); //(int fadeMins, int onTimeMins,uint8_t maxBrightness) //fades in blue LEDs, stays on for onTimeMins minutes, fades out

    void playWakeSound();
    void playSleepSound();
    void stopSound();
    void playSoundFor(int soundIndex, int secs); //play sound with index soundIndex for secs seconds

    bool isLampOn(); //returns true if LEDs are on, else returns false
    bool isWakeSeqTriggered(); //returns true if curr time = (wake time - half an hour), else false
    bool isSleepSeqTriggered(); //returns true if curr time = (sleep time - half an hour), else false

    uint8_t getInitHr(); //returns initial RTC hour
    uint8_t getInitMins(); //returns initial RTC minutes
    uint8_t getCurrHr(); //returns current hour
    uint8_t getCurrMins(); //returns current minutes
    uint8_t getWakeHr(); //returns currently set wake hr
    uint8_t getWakeMins(); //returns currently set wake minutes
    uint8_t getSleepHr(); //returns currently set sleep hr
    uint8_t getSleepMins(); //returns currently set wake minutes

    void setInitTime(uint8_t hr, uint8_t mins); //sets initial RTC time
    void setInitHr(uint8_t hr); //sets initial RTC hour
    void setInitMins(uint8_t mins); //sets initial RTC minutes
    void setWakeHr(uint8_t hr); //sets wake sequence trigger hour
    void setWakeMins(uint8_t mins);//sets wake sequence trigger minutes
    void setSleepHr(uint8_t hr); //sets sleep sequence trigger hour
    void setSleepMins(uint8_t mins);//sets wake sequence trigger minutes

    void setCurrColor(/*smartSleep::Colors*/ int c); //sets lamp color in idle mode
    void setCurrBrightnessLvl(/*smartSleep::BrightnessLevels*/ uint8_t b); //sets lamp brightness in idle mode
    /*smartSleep::Colors*/ int getCurrColor(); //get currently set lamp color
    /*smartSleep::BrightnessLevels*/ uint8_t getCurrBrightnessLvl(); //get currently set lamp brightness

    void setSelectedMenu(int index);
    int getSelectedMenu();
    void setWakeSound(int index);
    int getWakeSound();
    void setSleepSound(int index);
    int getSleepSound();
    void setSoundVolume(int volume);
    int getSoundVolume();

//private:
};



#endif //SMARTSLEEP_H
