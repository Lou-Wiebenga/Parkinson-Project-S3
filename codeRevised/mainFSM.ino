#include <Arduino.h>
#include <Wire.h>
#include <smartSleep.h>

#define BUTTON_DEBOUNCE_MILLIS 50
#define BUTTON_AMOUNT          6
#define ONOFF_BUTTON_PIN    13
#define SELECT_BUTTON_PIN    12
#define MENU_BUTTON_PIN    14
#define PLUS_BUTTON_PIN    27
#define MINUS_BUTTON_PIN    26
#define SNOOZE_BUTTON_PIN 100 //<CHANGE THIS

unsigned long prevMillisButton = 0;

bool initFinished = false;

enum Buttons {
  B_ONOFF,
  B_SELECT,
  B_MENU,
  B_PLUS,
  B_MINUS,
  B_SNOOZE
};

int buttonCurrStates[BUTTON_AMOUNT] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
int buttonPrevStates[BUTTON_AMOUNT] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
bool buttonPressed[BUTTON_AMOUNT]   = {false, false, false, false, false, false};
int buttonPins[BUTTON_AMOUNT]       = {ONOFF_BUTTON_PIN, SELECT_BUTTON_PIN, MENU_BUTTON_PIN, PLUS_BUTTON_PIN, MINUS_BUTTON_PIN, SNOOZE_BUTTON_PIN};

enum inputMode {
  I_HR,
  I_MINS,
  I_WAKE_SOUND,
  I_SLEEP_SOUND,
  I_VOLUME,
  I_COLOR,
  I_BRIGHTNESS
};

inputMode inpMode = I_HR;

enum States {
  S_WELCOME,
  S_TIME_SYNC,
  S_WAKE_TIME_SETUP,
  S_SLEEP_TIME_SETUP,
  S_SOUND_SETUP,
  S_LIGHT_SETUP,
  S_IDLE,
  S_MENUS,
  S_SLEEP,
  S_WAKE
};

enum States currState = S_WELCOME;

smartSleep wekker;

void setup() {

  Serial.begin(115200);

  pinMode(PLUS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MINUS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MENU_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ONOFF_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SNOOZE_BUTTON_PIN, INPUT_PULLUP);

  Wire.begin();
  Wire.setClock(100000);

  wekker.init();

  Serial.print("Hour: ");
  Serial.println(wekker.getCurrHr());
  Serial.print("Minutes: ");
  Serial.println(wekker.getCurrMins());

  wekker.lampOff();

}

void loop() {

  unsigned long currMillis = millis();

  if (buttonPressed[B_ONOFF]) {
    ;
  }

  if (buttonPressed[B_SNOOZE]) {
    wekker.stopSound();
    buttonPressed[B_SNOOZE] = false;
    if (currState == S_SLEEP || currState == S_WAKE) {
      wekker.lampOff();
      currState = S_IDLE;
      }
    else {
      if (wekker.isLampOn()) {
        wekker.lampOff();
      } else {
        wekker.lampOn(wekker.currColor, wekker.currBrightnessLvl);
      }
    }
    buttonPressed[B_ONOFF] = false;
  }

  switch(currState) {


    case S_WELCOME:

      wekker.lcdPrint("Welcome!\nPress MENU");
      wekker.lcdShow();
      wekker.lcdHome();

      if (buttonPressed[B_MENU]) {
        currState = S_TIME_SYNC;
        buttonPressed[B_MENU] = false;
        wekker.lcdHome();
      }
      break;

    case S_TIME_SYNC:

      wekker.lcdPrint("Current time:\n");
      wekker.lcdPrintTime(wekker.getInitHr(), wekker.getInitMins());
      wekker.lcdShow();
      wekker.lcdHome();

      if (buttonPressed[B_SELECT]) {
        if (inpMode == I_HR) {inpMode = I_MINS;} else {inpMode = I_HR;}
        buttonPressed[B_SELECT] = false;
      }

      if (buttonPressed[B_PLUS]) {
        switch (inpMode) {
          case I_HR:
            if (wekker.getInitHr() == 23) {wekker.setInitHr(0);} 
            else {wekker.setInitHr(wekker.getInitHr() + 1);}
            break;
          case I_MINS:
            if (wekker.getInitMins() == 59) {wekker.setInitMins(0);}
            else {wekker.setInitMins(wekker.getInitMins() + 1);}
            break;
        }
        buttonPressed[B_PLUS] = false;
      }

      if (buttonPressed[B_MINUS]) {
        switch (inpMode) {
          case I_HR:
            if (wekker.getInitHr() == 0) {wekker.setInitHr(23);} 
            else {wekker.setInitHr(wekker.getInitHr() - 1);}
            break;
          case I_MINS:
            if (wekker.getInitMins() == 0) {wekker.setInitMins(59);}
            else {wekker.setInitMins(wekker.getInitMins() - 1);}
            break;
        }
        buttonPressed[B_MINUS] = false;
      }

      if (buttonPressed[B_MENU]) {
        wekker.setInitTime(wekker.getInitHr(), wekker.getInitMins());
        wekker.lcdHome();
        inpMode = I_HR;
        if (initFinished) {wekker.lcdSetTextSize(3); currState=S_IDLE;} else {currState=S_WAKE_TIME_SETUP;}
        buttonPressed[B_MENU] = false;
      }
      break;


    case S_WAKE_TIME_SETUP:
    
        wekker.lcdPrint("Wake time:\n"); 
        wekker.lcdPrintTime(wekker.getWakeHr(), wekker.getWakeMins());
        wekker.lcdShow();
        wekker.lcdHome();

      if (buttonPressed[B_SELECT]) {
        if (inpMode == I_HR) {inpMode = I_MINS;} else {inpMode = I_HR;}
        buttonPressed[B_SELECT] = false;
      }

      if (buttonPressed[B_PLUS]) {
        switch (inpMode) {
          case I_HR:
            if (wekker.getWakeHr() == 23) {wekker.setWakeHr(0);} 
            else {wekker.setWakeHr(wekker.getWakeHr() + 1);}
            break;
          case I_MINS:
            if (wekker.getWakeMins() == 55) {wekker.setWakeMins(0);}
            else {wekker.setWakeMins(wekker.getWakeMins() + 5);}
            break;
        }
        buttonPressed[B_PLUS] = false;
      }

      if (buttonPressed[B_MINUS]) {
        switch (inpMode) {
          case I_HR:
            if (wekker.getWakeHr() == 0) {wekker.setWakeHr(23);} 
            else {wekker.setWakeHr(wekker.getWakeHr() - 1);}
            break;
          case I_MINS:
            if (wekker.getWakeMins() == 0) {wekker.setWakeMins(55);}
            else {wekker.setWakeMins(wekker.getWakeMins() - 5);}
            break;
        }
        buttonPressed[B_MINUS] = false;
      }

      if (buttonPressed[B_MENU]) {
        wekker.lcdHome();
        inpMode = I_HR;
        currState = S_SLEEP_TIME_SETUP;
        buttonPressed[B_MENU] = false;
      }
      break;

    case S_SLEEP_TIME_SETUP:

      wekker.lcdPrint("Sleep time:\n");
      wekker.lcdPrintTime(wekker.getSleepHr(), wekker.getSleepMins());
      wekker.lcdShow();
      wekker.lcdHome();

      if (buttonPressed[B_SELECT]) {
        if (inpMode == I_HR) {inpMode = I_MINS;} else {inpMode = I_HR;}
        buttonPressed[B_SELECT] = false;
      }

      if (buttonPressed[B_PLUS]) {
        switch (inpMode) {
          case I_HR:
            if (wekker.getSleepHr() == 23) {wekker.setSleepHr(0);} 
            else {wekker.setSleepHr(wekker.getSleepHr() + 1);}
            break;
          case I_MINS:
            if (wekker.getSleepMins() == 55) {wekker.setSleepMins(0);}
            else {wekker.setSleepMins(wekker.getSleepMins() + 5);}
            break;
        }
        buttonPressed[B_PLUS] = false;
      }

      if (buttonPressed[B_MINUS]) {
        switch (inpMode) {
          case I_HR:
            if (wekker.getSleepHr() == 0) {wekker.setSleepHr(23);} 
            else {wekker.setSleepHr(wekker.getSleepHr() - 1);}
            break;
          case I_MINS:
            if (wekker.getSleepMins() == 0) {wekker.setSleepMins(55);}
            else {wekker.setSleepMins(wekker.getSleepMins() - 5);}
            break;
        }
        buttonPressed[B_MINUS] = false;
      }
      
      if (buttonPressed[B_MENU]) {
        wekker.lcdHome();
        if (initFinished) {wekker.lcdSetTextSize(3); currState=S_IDLE;} else {wekker.lcdSetTextSize(1); inpMode = I_WAKE_SOUND; currState=S_SOUND_SETUP;}
        buttonPressed[B_MENU] = false;
      }

      break;

    case S_SOUND_SETUP:

      wekker.lcdPrint("Sound settings:\n");
      wekker.showSoundScreen();
      wekker.lcdShow();
      wekker.lcdHome();

      if (buttonPressed[B_SELECT]) {
        switch (inpMode) {
          case I_WAKE_SOUND:
            inpMode = I_SLEEP_SOUND;
            break;
          case I_SLEEP_SOUND:
            inpMode = I_VOLUME;
            break;
          case I_VOLUME:
            inpMode = I_WAKE_SOUND;
            break;
        }
        buttonPressed[B_SELECT] = false;
      }

      else if (buttonPressed[B_PLUS]) {
        switch (inpMode) {
          case I_WAKE_SOUND:
            if (wekker.getWakeSound() == SOUND_FILES_AMOUNT-1) {;}
            else {wekker.setWakeSound(wekker.getWakeSound() + 1);}
            break;
          
          case I_SLEEP_SOUND:
            if (wekker.getSleepSound() == SOUND_FILES_AMOUNT-1) {;}
            else {wekker.setSleepSound(wekker.getSleepSound() + 1);}
            break;

          case I_VOLUME:
            if (wekker.getSoundVolume() == MAX_VOLUME) {;}
            else {wekker.setSoundVolume(wekker.getSoundVolume() + 5);}
            break;
        }
        buttonPressed[B_PLUS] = false;
      }

      else if (buttonPressed[B_MINUS]) {
        switch (inpMode) {
          case I_WAKE_SOUND:
            if (wekker.getWakeSound() == 0) {;}
            else {wekker.setWakeSound(wekker.getWakeSound() - 1);}
            break;
          
          case I_SLEEP_SOUND:
            if (wekker.getSleepSound() == 0) {;}
            else {wekker.setSleepSound(wekker.getSleepSound() - 1);}
            break;

          case I_VOLUME:
            if (wekker.getSoundVolume() == 0) {;}
            else {wekker.setSoundVolume(wekker.getSoundVolume() - 5);}
            break;
        }
        buttonPressed[B_MINUS] = false;
      }

      else if (buttonPressed[B_MENU]) {
        wekker.lcdHome();
        wekker.lcdSetTextSize(3);
        if (!initFinished) {initFinished = true;}
        currState = S_IDLE;
        buttonPressed[B_MENU] = false;
      }

      break;

    case S_LIGHT_SETUP:

      wekker.showLightScreen();
      wekker.lcdShow();
      wekker.lcdHome();

      if (buttonPressed[B_SELECT]) {
        if (inpMode==I_COLOR) {inpMode=I_BRIGHTNESS;} else {inpMode=I_COLOR;}
        buttonPressed[B_SELECT] = false;
      }

      else if (buttonPressed[B_PLUS]) {
        if (inpMode==I_BRIGHTNESS) {
          if (wekker.getCurrBrightnessLvl() == HI) {;} 
          else { wekker.setCurrBrightnessLvl((wekker.getCurrBrightnessLvl()) + 100); }
          wekker.lampOn(wekker.currColor, wekker.currBrightnessLvl);
        }
        else {
          if (wekker.getCurrColor() == WARM_WHITE) {;}
          else { wekker.setCurrColor((wekker.getCurrColor()) + 1); }
          wekker.lampOn(wekker.currColor, wekker.currBrightnessLvl);
        }
        buttonPressed[B_PLUS] = false;
      }

      else if (buttonPressed[B_MINUS]) {
        if (inpMode==I_BRIGHTNESS) {
          if (wekker.getCurrBrightnessLvl() == LO) {;} 
          else { wekker.setCurrBrightnessLvl((wekker.getCurrBrightnessLvl()) - 100); }
          wekker.lampOn(wekker.currColor, wekker.currBrightnessLvl);
        }
        else {
          if (wekker.getCurrColor() == RED) {;}
          else { wekker.setCurrColor((wekker.getCurrColor()) - 1); }
          wekker.lampOn(wekker.currColor, wekker.currBrightnessLvl);
        }
        buttonPressed[B_MINUS] = false;
      }

      else if (buttonPressed[B_MENU]) {
        wekker.lcdHome();
        wekker.lcdSetTextSize(3);
        currState = S_IDLE;
        buttonPressed[B_MENU] = false;
      }

      break;
    
    case S_IDLE:

      wekker.lcdSetCursor(15, 15);
      wekker.lcdPrintCurrTime();
      wekker.lcdShow();
      wekker.lcdHome();

      //if ((wekker.getSleepHr() == wekker.getCurrHr()) && (wekker.getSleepMins() == wekker.getCurrMins())) {
      if (wekker.isSleepSeqTriggered()) {
        wekker.lcdHome();
        if (wekker.getSleepSound() == 0) {;} else {wekker.playSleepSound();}
        currState = S_SLEEP;
      }

      //else if ((wekker.getWakeHr() == wekker.getCurrHr()) && (wekker.getWakeMins() == wekker.getCurrMins())) {
      else if (wekker.isWakeSeqTriggered()) {
        wekker.lcdHome();
        currState = S_WAKE;
      }

      else if (buttonPressed[B_MENU]) {
        wekker.lcdHome();
        wekker.lcdSetTextSize(1);
        currState = S_MENUS;
        buttonPressed[B_MENU] = false;
      }

      break;

    case S_MENUS:

      wekker.lcdPrint("Settings:\n");
      wekker.showMenusScreen();
      wekker.lcdShow();
      wekker.lcdHome();

      if (buttonPressed[B_SELECT]) {
        if (wekker.getSelectedMenu() == (MENU_AMOUNT-1)) {
          wekker.setSelectedMenu(0);
        }
        else {
        wekker.setSelectedMenu(wekker.getSelectedMenu() + 1);
        }
        buttonPressed[B_SELECT] = false;
      }
      
      else if (buttonPressed[B_MENU]) {
        switch(wekker.getSelectedMenu()) {
          case M_CALIB_TIME:
            wekker.lcdSetTextSize(2);
            inpMode = I_HR;
            currState = S_TIME_SYNC;
            break;
          case M_SLEEP_WAKE_TIME:
            wekker.lcdSetTextSize(2);
            inpMode = I_HR;
            currState = S_WAKE_TIME_SETUP;
            break;
          case M_SOUND:
            inpMode = I_WAKE_SOUND;
            currState = S_SOUND_SETUP;
            break;
          case M_LIGHT:
            inpMode = I_COLOR;
            currState = S_LIGHT_SETUP;
            break;
        }
        wekker.lcdHome();
        buttonPressed[B_MENU] = false;
      }

      else if (buttonPressed[B_SNOOZE]) {
        wekker.lcdHome();
        wekker.lcdSetTextSize(3);
        currState = S_IDLE;
        buttonPressed[B_SNOOZE] = false;
      }

    break;

    case S_SLEEP:
 
      wekker.lcdSetCursor(15, 15);
      wekker.lcdPrintCurrTime();
      wekker.lcdShow();
      wekker.lcdHome();

      if (wekker.lampSleepSequence()) {
        currState = S_IDLE;
      }

      break;

    case S_WAKE:
 
      wekker.lcdSetCursor(15, 15);
      wekker.lcdPrintCurrTime();
      wekker.lcdShow();
      wekker.lcdHome();

      if (wekker.lampWakeSequence()) {
        if (wekker.getWakeSound() == 0) {;} else {wekker.playWakeSound();}
        currState = S_IDLE;
      }

      break;

    default:
      wekker.lampOn(RED, LO);
  }

  for (int i = 0; i < BUTTON_AMOUNT; i++) {
    buttonCurrStates[i] = digitalRead(buttonPins[i]);
  }

  if ((currMillis - prevMillisButton) >= BUTTON_DEBOUNCE_MILLIS) {
    for (int i = 0; i < BUTTON_AMOUNT; i++) {
      if ((buttonCurrStates[i] != buttonPrevStates[i]) && (buttonCurrStates[i] == HIGH)) {
        buttonPressed[i] = true;
      }
      buttonPrevStates[i] = buttonCurrStates[i];
    }
    prevMillisButton = currMillis;
  
  }
  
}
