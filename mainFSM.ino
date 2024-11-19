#include <Arduino.h>
#include <Wire.h>
#include <smartSleep.h>

#define BUTTON_DEBOUNCE_MILLIS 50
#define BUTTON_AMOUNT          5
#define ONOFF_BUTTON_PIN    13
#define SELECT_BUTTON_PIN    12
#define MENU_BUTTON_PIN    14
#define PLUS_BUTTON_PIN    27
#define MINUS_BUTTON_PIN    26
#define INT_PIN 5

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

int cnt = 0;

smartSleep wekker;

/*void RTC_INT_Trigger() {
  volatile uint8_t RTC_INT = 1;
  cnt++;
  if (cnt == 5){
    Wire.beginTransmission(0x51);
    Serial.print(RTC_readTime(Minutes));
    cnt = 0;
    Wire.endTransmission(0x51);
  }
}*/

void setup() {

  Serial.begin(115200);

  //pinMode(INT_PIN, INPUT);        // set up interrupt pin
  //digitalWrite(INT_PIN, HIGH);    // turn on pullup resistors
  //attachInterrupt(digitalPinToInterrupt(INT_PIN), RTC_INT_Trigger, RISING);

  pinMode(PLUS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MINUS_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MENU_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ONOFF_BUTTON_PIN, INPUT_PULLUP);

  Wire.begin();
  Wire.setClock(100000);

  //Wire.beginTransmission(0x27);
  //Wire.beginTransmission(0x51);
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
        buttonPressed[B_SELECT] = false;
      }

      if (buttonPressed[B_PLUS]) {
        switch (inputMode) {
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
        switch (inputMode) {
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
        buttonPressed[B_SELECT] = false;
      }

      if (buttonPressed[B_PLUS]) {
        switch (inputMode) {
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
        switch (inputMode) {
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
        currState = S_IDLE;
        buttonPressed[B_MENU] = false;
        wekker.lcdClear();
        wekker.lcdInputModeOff();
      }
      break;

    case S_IDLE:

      wekker.lcdSetCursor(1, 2);
      
      //RTC_readTime(Minutes);
      
      Serial.println(RTC_readTime(Hour));
      Serial.println("hour");
      Serial.println(RTC_readTime(Minutes));
      Serial.println("mins");

      wekker.lcdPrintCurrTime(2);
      wekker.lcdHome();
      
      /*if ((wekker.getSleepHr() == wekker.getCurrHr()) && (wekker.getSleepMins() == wekker.getCurrMins())) {
        currState = S_SLEEP;
      }

      else if ((wekker.getWakeHr() == wekker.getCurrHr()) && (wekker.getWakeMins() == wekker.getCurrMins())) {
        currState = S_WAKE;
      }*/

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

