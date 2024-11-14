//
// Created by annza on 10/7/2024.
//

#include "smartSleep.h"

smartSleep::smartSleep() : lcd(SS_LCD_ADDRESS), leds(SS_LED_COUNT, LED_DATA_PIN, NEO_GRB + NEO_KHZ800) {
    currColor = YELLOW;
    currBrightnessLvl = LO;
    lampTurnedOn = false;
    wakeHr = 7;
    wakeMin = 0;
    sleepHr = 23;
    sleepMin = 0;
}

void smartSleep::init() {

  	RTC_init();
    RTC_setTimeAuto();

    leds.begin();
    leds.clear();

    lcd.begin(SS_LCD_COLUMNS, SS_LCD_ROWS);
    lcd.clear();
    lcd.display();
    delay(1000);
    lcd.noDisplay();
    delay(1000);
    lcd.display();
    lcd.home();
}

void smartSleep::lcdPrint(const char* text) {
  lcd.print(text);
}

void smartSleep::lcdPrintCenter(uint8_t row, const char* text) {
  lcd.center(row, text);
}

void smartSleep::lcdClear() {
  lcd.clear();
  lcd.home();
}

void smartSleep::lcdSetCursor(uint8_t col, uint8_t row) {
  lcd.setCursor(col, row);
}

void smartSleep::lcdPrintTime(uint8_t row, uint8_t hour, uint8_t mins) {
/*TO BE IMPLEMNTED
 (possible solutions below)

--No1:
 char buf[5];
 itoa(hour, buf, 10);
  ...

--No2:
  use .toString
 */
}

void smartSleep::lcdPrintCurrTime(uint8_t row) {			//NOT TESTED
  lcdPrintCenter(row, RTC_getHhMmString());
}

void smartSleep::lampFadeIn (int mins, uint8_t maxBrightness, /*smartSleep::Colors*/ int color) {
    unsigned long currMillis = 0;
    unsigned long prevMillis = 0;
    unsigned long delayMs = (mins * 60 * 1000) / maxBrightness;
    uint8_t pBrightness = 0; //pixel brightness

    leds.clear();

    while (pBrightness != maxBrightness) {
      currMillis = millis();
      if ((currMillis - prevMillis) >= delayMs) {
        prevMillis = currMillis;
        lampOn(color, pBrightness);
        pBrightness++;
      }
    }
}

void smartSleep::lampFadeOut(int mins, uint8_t maxBrightness, /*smartSleep::Colors*/ int color) {
    unsigned long currMillis = 0;
    unsigned long prevMillis = 0;
    unsigned long delayMs = (mins * 60 * 1000) / maxBrightness;
    uint8_t pBrightness = maxBrightness; //pixel brightness

    leds.clear();

    while (pBrightness != 0) {
      currMillis = millis();
      if ((currMillis - prevMillis) >= delayMs) {
        prevMillis = currMillis;
        lampOn(color, pBrightness);
        pBrightness--;
      }
    }
}

void smartSleep::lampSleepSequence(int onTimeMins, uint8_t maxBrightness) {
    unsigned long currMillis = 0;
    unsigned long delayMs = (onTimeMins * 60 * 1000);

    leds.clear();

    lampFadeIn(5, maxBrightness, RED);

    currMillis = millis();
    if (currMillis >= delayMs) {
        lampFadeOut(5, maxBrightness, RED);
    }
}

void smartSleep::lampWakeSequence(int onTimeMins, uint8_t maxBrightness) {
    unsigned long currMillis = 0;
    unsigned long delayMs = (onTimeMins * 60 * 1000);

    leds.clear();

    lampFadeIn(5, maxBrightness, BLUE);

    currMillis = millis();
    if (currMillis >= delayMs) {
        lampFadeOut(5, maxBrightness, BLUE);
    }
}

void smartSleep::lampOn(/*smartSleep::Colors*/ int c, uint8_t brightness) {

  uint32_t color;

  switch (c) {
    case RED:
      color = leds.Color(brightness, 0, 0);
      break;
    case ORANGE:
      color = leds.Color(brightness, brightness, 0);
      break;
    case YELLOW:
      color = leds.Color(brightness, brightness, 0);
      break;
    case GREEN:
      color = leds.Color(0, brightness, 0);
      break;
    case BLUE:
      color = leds.Color(0, 0, brightness);
      break;
    case PURPLE:
      color = leds.Color(brightness, 0, brightness);
      break;
    case WHITE:
      color = leds.Color(brightness, brightness, brightness);
      break;
    case WARM_WHITE:
      color = leds.Color(brightness, brightness, brightness);
      break;
    default:
      ;
  }

    for (int i=0; i < SS_LED_COUNT; i++) {
      leds.setPixelColor(i, color);
      leds.show();
    }
  lampTurnedOn = true;
}

void smartSleep::lampOff() {
  leds.clear();
  leds.show();
  lampTurnedOn = false;
}

bool smartSleep::isLampOn() {
  return lampTurnedOn;
}

uint8_t smartSleep::getCurrHr() {
  return RTC_readTime(Hour);
}

uint8_t smartSleep::getCurrMins() {
  return RTC_readTime(Minutes);
}

void smartSleep::setWakeTime(uint8_t hr, uint8_t mins) {
  wakeHr = hr;
  wakeMin = mins;
}

void smartSleep::setSleepTime(uint8_t hr, uint8_t mins) {
  sleepHr = hr;
  sleepMin = mins;
}

void smartSleep::setCurrColor(/*smartSleep::Colors*/ int c) {
  currColor = c;
}

void smartSleep::setCurrBrightnessLvl(/*smartSleep::BrightnessLevels*/ uint8_t b) {
  currBrightnessLvl = b;
}

/*smartSleep::Colors*/ int smartSleep::getCurrColor() {
  return currColor;
}

/*smartSleep::BrightnessLevels*/ uint8_t smartSleep::getCurrBrightnessLvl() {
  return currBrightnessLvl;
}