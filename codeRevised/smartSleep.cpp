//
// Created by annza on 10/7/2024.
//

#include "smartSleep.h"

smartSleep::smartSleep() : lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1), leds(SS_LED_COUNT, LED_DATA_PIN, NEO_GRB + NEO_KHZ800) {
    currColor = YELLOW;
    currBrightnessLvl = MED;
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

    if(!lcd.begin(SSD1306_SWITCHCAPVCC, SS_LCD_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
    }

    lcd.display();

    lcd.clearDisplay();
    lcd.cp437(true);
    lcd.setTextColor(SSD1306_WHITE);
    lcd.setTextSize(2);
    /*
    lcd.begin(SS_LCD_COLUMNS, SS_LCD_ROWS);
    lcd.clear();
    lcd.display();
    delay(1000);
    lcd.noDisplay();
    delay(1000);
    lcd.display();
    lcdHome()();
     */

    //UNCOMMENT WHEN MP3 MODULE IS CONNECTED!
    //speakersSerial.begin(9600, SERIAL_8N1, 3, 1);
    //speakers.begin(speakersSerial, /*isACK = */true, /*doReset = */true);
    //speakers.setTimeOut(500);
    //speakers.volume(10);
    //speakers.EQ(DFPLAYER_EQ_NORMAL);
    //speakers.outputDevice(DFPLAYER_DEVICE_SD);
}


//void smartSleep::lcdPrint(T text)

void smartSleep::lcdPrintCenter(uint8_t row, const char* text) {
  //lcd.center(row, text);
  ;
}

void smartSleep::lcdClear() {
  lcd.clearDisplay();
  lcdHome();
  lcd.display();
  //lcd.clear();
  //lcdHome();
}

void smartSleep::lcdSetTextSize(uint8_t size) {
  lcd.setTextSize(size);
}

void smartSleep::lcdSetCursor(uint8_t col, uint8_t row) {
  lcd.setCursor(col, row);
}

void smartSleep::lcdHome() {
  lcd.clearDisplay();
  lcd.setCursor(0, 0);
  //lcdHome()();
}

void smartSleep::lcdPrintTime(uint8_t hour, uint8_t mins) {
  if (hour < 10) {lcd.print(0);}
  lcd.print(hour);
  lcd.print(":");
  if (mins < 10) {lcd.print(0);}
  lcd.print(mins);
}

void smartSleep::lcdPrintCurrTime() {

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

void smartSleep::showMenusScreen() {
//  if (selectedMenu%2==0) {
//      if(selectedMenu<(MENU_AMOUNT-1)) {
//      lcd.print('>');
//      lcd.println(menus[selectedMenu]);
//      lcd.print(menus[selectedMenu+1]);
//      }
//      else {lcd.print('>'); lcd.println(menus[selectedMenu]);}
//  }
//  if (selectedMenu%2==1 && selectedMenu<=(MENU_AMOUNT-1)) {
//    lcd.println(menus[selectedMenu-1]);
//    lcd.print('>');
//    lcd.print(menus[selectedMenu]);
//  }
    for (int i=0; i < selectedMenu; i++) {
      lcd.println(menus[i]);
    }
    lcd.print('>');
    lcd.println(menus[selectedMenu]);

    for (int i=selectedMenu+1; i < MENU_AMOUNT; i++) {
      lcd.println(menus[i]);
    }
}

void smartSleep::showSoundScreen() {
  lcd.print("Wake: ");
  if (wakeSound == 0) {lcd.print(sounds[wakeSound]); lcd.println('>');}
  else if (wakeSound == SOUND_FILES_AMOUNT-1) {lcd.print('<');lcd.println(sounds[wakeSound]);}
  else {lcd.print('<'); lcd.print(sounds[wakeSound]); lcd.println('>');}

  lcd.print("Sleep: ");
  if (sleepSound == 0) {lcd.print(sounds[sleepSound]); lcd.println('>');}
  else if (sleepSound == SOUND_FILES_AMOUNT-1) {lcd.print('<');lcd.println(sounds[sleepSound]);}
  else {lcd.print('<'); lcd.print(sounds[sleepSound]); lcd.println('>');}

  lcd.print("Volume: ");
  for (int i = 0; i<soundVolume; i=i+5) {lcd.print('#');}
}

void smartSleep::showLightScreen() {
  lcd.print("Color: ");
  if (currColor == 0) {lcd.print(colors[currColor]); lcd.println('>');}
  else if (currColor == COLORS_AMOUNT-1) {lcd.print('<');lcd.println(colors[currColor]);}
  else {lcd.print('<'); lcd.print(colors[currColor]); lcd.println('>');}

  lcd.print("Brightness: ");
  for (int i = 55; i<=currBrightnessLvl; i=i+100) {lcd.print('#');}
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

int smartSleep::lampSleepSequence() {
//    unsigned long currMillis = 0;
//    unsigned long prevMillis = 0;
//    unsigned long delayMs = onTimeMins * 60 * 1000;
//
//    leds.clear();
//
//    lampFadeIn(fadeMins, maxBrightness, RED);
//
//    currMillis = millis();
//    prevMillis = millis();
//
//    while ((currMillis - prevMillis) < delayMs) {
//      currMillis = millis();
//      lampOn(RED, maxBrightness);
//    }
//    lampFadeOut(fadeMins, maxBrightness, RED);

      if (((millis() - prevMillisFade)>= fadeDelayMs) && (pBrightness < HI) && !doFadeOut) {
        pBrightness++;
        lampOn(RED, pBrightness);
        prevMillisFade = millis();
        if (pBrightness==HI) {fadeOutPrevMillis=millis();}
      }

      if (((millis() - prevMillisFade)>= (RB_SEQ_MINS*60*1000 - 2*FADE_MINS*60*1000)) && (pBrightness == HI)) {
        doFadeOut = true;
      }

      if (((millis() - prevMillisFade)>= fadeDelayMs) && doFadeOut) {
        pBrightness--;
        lampOn(RED, pBrightness);
        if (pBrightness==0) {
          doFadeOut = false;
          lampOff();
          return 1;
        }
        prevMillisFade = millis();
      }

      return 0;
}

int smartSleep::lampWakeSequence() {
//    unsigned long currMillis = 0;
//    unsigned long prevMillis = 0;
//    unsigned long delayMs = onTimeMins * 60 * 1000;
//
//    leds.clear();
//
//    lampFadeIn(fadeMins, maxBrightness, BLUE);
//
//    currMillis = millis();
//    prevMillis = millis();
//
//    while ((currMillis - prevMillis) < delayMs) {
//      currMillis = millis();
//      lampOn(BLUE, maxBrightness);
//    }
//    lampFadeOut(fadeMins, maxBrightness, BLUE);

      if (((millis() - prevMillisFade)>= fadeDelayMs) && (pBrightness < HI) && !doFadeOut) {
        pBrightness++;
        lampOn(BLUE, pBrightness);
        prevMillisFade = millis();
        if (pBrightness==HI) {fadeOutPrevMillis=millis();}
      }

      if (((millis() - prevMillisFade)>= (RB_SEQ_MINS*60*1000 - 2*FADE_MINS*60*1000)) && (pBrightness == HI)) {
        doFadeOut = true;
      }

      if (((millis() - prevMillisFade)>= fadeDelayMs) && doFadeOut) {
        pBrightness--;
        lampOn(BLUE, pBrightness);
        if (pBrightness==0) {
          doFadeOut = false;
          lampOff();
          return 1;
        }
        prevMillisFade = millis();
      }
      return 0;
}

void smartSleep::playWakeSound() {
  speakers.play(getWakeSound() + 1);
}

void smartSleep::playSleepSound() {
  speakers.play(getSleepSound() + 1);
}

void smartSleep::stopSound() {
  speakers.pause();
}

//void smartSleep::playSoundFor(int soundIndex, int secs)

bool smartSleep::isLampOn() {
  return lampTurnedOn;
}

bool smartSleep::isWakeSeqTriggered() {
  uint8_t adjustedHr = wakeHr;
  uint8_t adjustedMins = wakeMins;
  if (adjustedMins < 30) {
    if (adjustedHr == 0) {adjustedHr = 23;} else {adjustedHr--;}
    adjustedMins = adjustedMins + 30;
  }
  else {
    adjustedMins = adjustedMins - 30;
  }
  if ((RTC_readTime(Hour) == adjustedHr) && (RTC_readTime(Minutes) == adjustedMins)) {
    return true;
  }
  return false;
}

bool smartSleep::isSleepSeqTriggered() {
  uint8_t adjustedHr = sleepHr;
  uint8_t adjustedMins = sleepMins;
  if (adjustedMins < 30) {
    if (adjustedHr == 0) {adjustedHr = 23;} else {adjustedHr--;}
    adjustedMins = adjustedMins + 30;
  }
  else {
    adjustedMins = adjustedMins - 30;
  }
  if ((RTC_readTime(Hour) == adjustedHr) && (RTC_readTime(Minutes) == adjustedMins)) {
    return true;
  }
  return false;
}

void smartSleep::setCurrColor(/*smartSleep::Colors*/ int c) {
  currColor = c;
}

void smartSleep::setCurrBrightnessLvl(/*smartSleep::BrightnessLevels*/ uint8_t b) {
  currBrightnessLvl = b;
  fadeDelayMs = FADE_MINS * 60 * 1000 / currBrightnessLvl;
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

void smartSleep::setSelectedMenu(int index) {
  selectedMenu = index;
}

int smartSleep::getSelectedMenu() {
  return selectedMenu;
}

void smartSleep::setWakeSound(int index) {
  wakeSound = index;
}

int smartSleep::getWakeSound() {
  return wakeSound;
}

void smartSleep::setSleepSound(int index) {
  sleepSound = index;
}

int smartSleep::getSleepSound() {
  return sleepSound;
}

void smartSleep::setSoundVolume(int volume) {
  soundVolume = volume;
}

int smartSleep::getSoundVolume() {
  return soundVolume;
}
