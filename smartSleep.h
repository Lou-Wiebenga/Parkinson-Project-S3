//
// Created by annza on 10/7/2024.
//

#ifndef SMARTSLEEP_H
#define SMARTSLEEP_H

#include <Adafruit_NeoPixel.h>
#include <I2C_LCD.h>

#define SS_LCD_ADDRESS 0x27
#define SS_LED_COUNT 12                    //amount of LEDs on ring
#define LED_DATA_PIN 2                  //pin on ESP32 connected to DIN on LED ring
#define LAMP_SLEEP_SEQ_MILLIS   5000    //how long the red light is on
#define LAMP_WAKE_SEQ_MILLIS    5000    //how long the blue light is on

class smartSleep {
public:
    smartSleep();

    void init();

    void timeCalibrate();

    void lampSleepSequence(int minutes);
    void lampWakeSequence(int minutes);
    void startAlarm();

    void lampColor(uint32_t color);

    void setLampOnTime();
    void setLampOffTime();
    void setAlarm();

    I2C_LCD lcd;
    Adafruit_NeoPixel leds;

//private:
    //unsigned long sSmillis = 0;
    //byte alarmH; --alarm hour
    //byte alarmM; --alarm minute
};



#endif //SMARTSLEEP_H
