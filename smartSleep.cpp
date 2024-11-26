//
// Created by annza on 10/7/2024.
//

#include "smartSleep.h"

smartSleep::smartSleep() : lcd(SS_LCD_ADDRESS), leds(SS_LED_COUNT, LED_DATA_PIN, NEO_GRB + NEO_KHZ800) {
    currColor = YELLOW;
    currBrightnessLvl = LO;
    lampTurnedOn = false;
    initHr = 12;
    initMins = 30;
    wakeHr = 7;
    wakeMins = 0;
    sleepHr = 23;
    sleepMins = 0;
}

void smartSleep::init() {

  	RTC_init();
    //RTC_setTimeAuto();

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

void smartSleep::lcdHome() {
  lcd.home();
}

void smartSleep::lcdPrintTime(uint8_t row, uint8_t hour, uint8_t mins) {
  lcd.setCursor(5, row);
  if (hour < 10) lcd.print(0);
  lcd.print(hour);
  lcd.print(":");
  if (mins < 10) lcd.print(0);
  lcd.print(mins);
}

void smartSleep::lcdPrintCurrTime() {			//NOT TESTED
  if (RTC_readTime(Hour) < 10) {
    lcd.print(0);
    lcd.print(RTC_readTime(Hour));
  }
  else {
      lcd.print(RTC_readTime(Hour));
  }

  lcd.print(":");

  if (RTC_readTime(Minutes) < 10) {
    lcd.print(0);
    lcd.print(RTC_readTime(Minutes));
  }
  else {
    lcd.print(RTC_readTime(Minutes));
  }
}

void smartSleep::lcdInputModeOn() {
  lcd.cursor();
  lcd.blink();
}

void smartSleep::lcdInputModeOff() {
  lcd.noCursor();
  lcd.noBlink();
}

void smartSleep::lampOn(/*smartSleep::Colors*/ int c, uint8_t brightness) {

  uint32_t color;

  switch (c) {
    case RED:
      color = leds.Color(brightness, 0, 0);
      break;
    case ORANGE:
      color = leds.Color(brightness, (brightness/2), 0);
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
      color = leds.Color(brightness, brightness, (brightness/2));
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
    lampOff();
}

void smartSleep::lampSleepSequence(int fadeMins, int onTimeMins, uint8_t maxBrightness) {
    unsigned long currMillis = 0;
    unsigned long prevMillis = 0;
    unsigned long delayMs = onTimeMins * 60 * 1000;

    leds.clear();

    lampFadeIn(fadeMins, maxBrightness, RED);

    currMillis = millis();
    prevMillis = millis();
    
    while ((currMillis - prevMillis) < delayMs) {
      currMillis = millis();
      lampOn(RED, maxBrightness);
    }
    //if (currMillis >= delayMs) {
        lampFadeOut(fadeMins, maxBrightness, RED);
    //}
}

void smartSleep::lampWakeSequence(int fadeMins, int onTimeMins, uint8_t maxBrightness) {
    unsigned long currMillis = 0;
    unsigned long prevMillis = 0;
    unsigned long delayMs = onTimeMins * 60 * 1000;

    leds.clear();

    lampFadeIn(fadeMins, maxBrightness, BLUE);

    currMillis = millis();
    prevMillis = millis();

    while ((currMillis - prevMillis) < delayMs) {
      currMillis = millis();
      lampOn(BLUE, maxBrightness);
    }
    //if (currMillis >= delayMs) {
        lampFadeOut(fadeMins, maxBrightness, BLUE);
    //}
}

bool smartSleep::isLampOn() {
  return lampTurnedOn;
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

uint8_t smartSleep::getInitHr() {
  return initHr;
}

uint8_t smartSleep::getInitMins() {
  return initMins;
}

uint8_t smartSleep::getCurrHr() {
  return RTC_readTime(Hour);
}

uint8_t smartSleep::getCurrMins() {
  return RTC_readTime(Minutes);
}

uint8_t smartSleep::getWakeHr() {
  return wakeHr;
}

uint8_t smartSleep::getWakeMins() {
  return wakeMins;
}

uint8_t smartSleep::getSleepHr() {
  return sleepHr;
}

uint8_t smartSleep::getSleepMins() {
  return sleepMins;
}

void smartSleep::setInitTime(uint8_t hr, uint8_t mins) {
  RTC_setTimeManual(2024, 11, 25, hr, mins, 0);
}

void smartSleep::setInitHr(uint8_t hr) {
  initHr = hr;
}

void smartSleep::setInitMins(uint8_t mins) {
  initMins = mins;
}

void smartSleep::setWakeHr(uint8_t hr) {
  wakeHr = hr;
}

void smartSleep::setWakeMins(uint8_t mins) {
  wakeMins = mins;
}

void smartSleep::setSleepHr(uint8_t hr) {
  sleepHr = hr;
}

void smartSleep::setSleepMins(uint8_t mins) {
  sleepMins = mins;
}

