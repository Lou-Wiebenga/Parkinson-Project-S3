#include <Arduino.h>
#include <Wire.h>
#include <smartSleep.h>

#define BUTTON_DEBOUNCE_MILLIS 50
#define BUTTON_AMOUNT          5
#define ONOFF_BUTTON_PIN    NULL
#define SELECT_BUTTON_PIN    5
#define MENU_BUTTON_PIN    4
#define PLUS_BUTTON_PIN    NULL
#define MINUS_BUTTON_PIN    NULL

unsigned long prevMillis = 0;

enum Buttons {
  B_ONOFF,
  B_SELECT,
  B_MENU,
  B_PLUS,
  B_MINUS
};

int buttonCurrStates[BUTTON_AMOUNT] = {HIGH, HIGH, HIGH, HIGH, HIGH};
int buttonPrevStates[BUTTON_AMOUNT] = {HIGH, HIGH, HIGH, HIGH, HIGH};
bool buttonPressed[BUTTON_AMOUNT]   = {false, false, false, false, false};
int buttonPins[BUTTON_AMOUNT]       = {ONOFF_BUTTON_PIN, SELECT_BUTTON_PIN, MENU_BUTTON_PIN, PLUS_BUTTON_PIN, MINUS_BUTTON_PIN};

enum timeInputMode {
  I_HR,
  I_MINS
};

timeInputMode inputMode = I_HR;

enum States {
  S_WELCOME,
  S_WAKE_TIME_SETUP,
  S_SLEEP_TIME_SETUP,
  S_IDLE,
  S_SLEEP,
  S_WAKE
};

enum States currState = S_WELCOME;

smartSleep wekker;

void setup() {

  pinMode(PLUS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MINUS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MENU_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ONOFF_BUTTON_PIN, INPUT_PULLUP);

  Wire.begin();
  Wire.setClock(100000);

  wekker.init();

}

void loop() {

  unsigned long currMillis = millis();

  switch(currState) {

    case S_WELCOME:

      wekker.lampOff();
      wekker.lcdSetCursor(1, 2);
      wekker.lcdPrint("Welcome!");
      wekker.lcdSetCursor(1, 1);
      wekker.lcdPrint("Press MENU");
      wekker.lcdHome();
      if (buttonPressed[B_MENU]) {
        currState = S_WAKE_TIME_SETUP;
        buttonPressed[B_MENU] = false;
        wekker.lcdClear();
        wekker.lcdInputModeOn();
      }
      break;

    case S_WAKE_TIME_SETUP:
    
      wekker.lcdSetCursor(1, 2);
      wekker.lcdPrint("Wake time:       ");
      wekker.lcdPrintTime(1, wekker.getWakeHr(), wekker.getWakeMins());
      wekker.lcdHome();

      if (buttonPressed[B_SELECT]) {
        if (inputMode == I_HR) {inputMode = I_MINS;} else {inputMode = I_HR;}
      }

      if (buttonPressed[B_PLUS]) {
        if(inputMode == I_HR) {wekker.setWakeHr(wekker.getWakeHr() + 1);}
        else {wekker.setWakeMins(wekker.getWakeMins() + 1);}
      }

      if (buttonPressed[B_MINUS]) {
        if(inputMode == I_HR) {wekker.setWakeHr(wekker.getWakeHr() - 1);}
        else {wekker.setWakeMins(wekker.getWakeMins() - 1);}
      }

      if (buttonPressed[B_MENU]) {
        currState = S_SLEEP_TIME_SETUP;
        buttonPressed[B_MENU] = false;
        wekker.lcdClear();
      }
      break;

    case S_SLEEP_TIME_SETUP:
      wekker.lcdSetCursor(1, 2);
      wekker.lcdPrint("Sleep time:      ");
      wekker.lcdPrintTime(1, wekker.getSleepHr(), wekker.getSleepMins());
      wekker.lcdHome();

      if (buttonPressed[B_SELECT]) {
        if (inputMode == I_HR) {inputMode = I_MINS;} else {inputMode = I_HR;}
      }

      if (buttonPressed[B_PLUS]) {
        if(inputMode == I_HR) {wekker.setSleepHr(wekker.getSleepHr() + 1);}
        else {wekker.setSleepMins(wekker.getSleepMins() + 1);}
      }

      if (buttonPressed[B_MINUS]) {
        if(inputMode == I_HR) {wekker.setSleepHr(wekker.getSleepHr() - 1);}
        else {wekker.setSleepMins(wekker.getSleepMins() - 1);}
      }
      
      if (buttonPressed[B_MENU]) {
        currState = S_IDLE;
        buttonPressed[B_MENU] = false;
        wekker.lcdClear();
        wekker.lcdInputModeOff();
      }
      break;

    case S_IDLE:

      if ((wekker.getSleepHr() == wekker.getCurrHr()) && (wekker.getSleepMins() == wekker.getCurrMins())) {
        currState = S_SLEEP;
      }

      else if ((wekker.getWakeHr() == wekker.getCurrHr()) && (wekker.getWakeMins() == wekker.getCurrMins())) {
        currState = S_WAKE;
      }

      if (buttonPressed[B_ONOFF]) {
        if (wekker.isLampOn()) {
          wekker.lampOff();
        } else {
          wekker.lampOn(wekker.currColor, wekker.currBrightnessLvl);
        }
        buttonPressed[B_ONOFF] = false;
      }

      else if (buttonPressed[B_PLUS]) {
        if (wekker.getCurrBrightnessLvl() == HI) {
          ;
        } else {
          wekker.setCurrBrightnessLvl((wekker.getCurrBrightnessLvl()) + 100);
        }
        wekker.lampOn(wekker.currColor, wekker.currBrightnessLvl);
        buttonPressed[B_PLUS] = false;
      }

      else if (buttonPressed[B_MINUS]) {
        if (wekker.getCurrBrightnessLvl() == LO) {
          ;
        } else {
          wekker.setCurrBrightnessLvl((wekker.getCurrBrightnessLvl()) - 100);
        }
        wekker.lampOn(wekker.currColor, wekker.currBrightnessLvl);
        buttonPressed[B_MINUS] = false;
      }

      else if (buttonPressed[B_SELECT]) {
        if (wekker.getCurrColor() == WARM_WHITE) {
          wekker.setCurrColor(RED);
        } else {
          wekker.setCurrColor((wekker.getCurrColor()) + 1);
        }
        wekker.lampOn(wekker.currColor, wekker.currBrightnessLvl);
        buttonPressed[B_SELECT] = false;
      } 

      else if (buttonPressed[B_MENU]) {
        currState = S_WAKE_TIME_SETUP;
        buttonPressed[B_MENU] = false;
      }

      break;

    case S_SLEEP:
      wekker.lampSleepSequence(30, wekker.getCurrBrightnessLvl());
      currState = S_IDLE;
      break;

    case S_WAKE:
      wekker.lampWakeSequence(30, wekker.getCurrBrightnessLvl());
      currState = S_IDLE;
      break;

    default:
      wekker.lampOn(RED, LO);
  }

  for (int i = 0; i < BUTTON_AMOUNT; i++) {
    buttonCurrStates[i] = digitalRead(buttonPins[i]);
  }

  if ((currMillis - prevMillis) >= BUTTON_DEBOUNCE_MILLIS) {
    for (int i = 0; i < BUTTON_AMOUNT; i++) {
      if ((buttonCurrStates[i] != buttonPrevStates[i]) && (buttonCurrStates[i] == HIGH)) {
        buttonPressed[i] = true;
      }
      buttonPrevStates[i] = buttonCurrStates[i];
    }
    prevMillis = currMillis;
  
  }


}
