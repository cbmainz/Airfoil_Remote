// Airfoil Bluetooth Remote
#define MY_DEBUG 0
#include "BluefruitRoutines.h"

//Pin numbers for switches
#define VOLUME_UP_SWITCH A0
#define VOLUME_DOWN_SWITCH A1
#define PLAY_PAUSE_SWITCH A2
#define NEXT_SWITCH A3

//Actions
#define DO_VOLUME_UP 1
#define DO_VOLUME_DOWN 2
#define DO_PLAY_PAUSE 4
#define DO_NEXT 8

uint8_t readSwitches(void) {
  return (~(digitalRead(VOLUME_UP_SWITCH)*DO_VOLUME_UP
      + digitalRead(VOLUME_DOWN_SWITCH)*DO_VOLUME_DOWN
      + digitalRead (PLAY_PAUSE_SWITCH)*DO_PLAY_PAUSE
      + digitalRead (NEXT_SWITCH)*DO_NEXT)
     ) & (DO_VOLUME_UP+ DO_VOLUME_DOWN+ DO_PLAY_PAUSE+ DO_NEXT);
}

//Translate character to keyboard keycode and transmit
void pressKeyCode (uint8_t c) {
  ble.print(F("AT+BLEKEYBOARDCODE=08-00-")); //sends additional left cmd key 
  uint8_t Code=c;
  ble.print(Code,HEX);
  ble.println(F("-00-00-00-00"));
  MESSAGE(F("Pressed."));
  delay(100);//de-bounce
  while (readSwitches()) { //wait for button to be released
    /*do nothing*/
  };
  ble.println(F("AT+BLEKEYBOARDCODE=00-00")); //release message
  MESSAGE(F("Released"));
}

void setup() {
#if(MY_DEBUG)
  while (! Serial) {}; delay (500);
  Serial.begin(9600); Serial.println("Debug output");
#endif
  pinMode(VOLUME_DOWN_SWITCH, INPUT_PULLUP);
  pinMode(PLAY_PAUSE_SWITCH, INPUT_PULLUP);
  pinMode(VOLUME_UP_SWITCH, INPUT_PULLUP);
  pinMode(NEXT_SWITCH, INPUT_PULLUP);
  initializeBluefruit();
}

void loop() {
  uint8_t i=readSwitches();
  switch (i) {
    case DO_VOLUME_UP:   pressKeyCode(0x80); break; //Volume up
    case DO_VOLUME_DOWN: pressKeyCode(0x81); break; //Volum down
    case DO_PLAY_PAUSE:  pressKeyCode(0x13); break; //Play/Pause
    case DO_NEXT:        pressKeyCode(0x17); break; //Next
  }
}
