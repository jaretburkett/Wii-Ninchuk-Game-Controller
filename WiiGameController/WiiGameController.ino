// Main Defines
#define AVG_READINGS 20     // the number of readings to average
#define POLLING_DELAY 100   // the number of ms to wait between polls

// includes
#include <Keyboard.h>
#include <Wire.h>
#include "nunchuk.h"
#include "Average.h"


bool isCPressed = false;
bool isZPressed = false;
bool isCtrlPressed = false;
bool isShiftPressed = false;
// joystick
bool isLeftPressed = false;
bool isRightPressed = false;
bool isUpPressed = false;
bool isDownPressed = false;

bool isLeanRightPressed = false;
bool isLeanLeftPressed = false;
//jump;
bool isJumpPressed = false;
int16_t previousAcclZ = 0;
int16_t previousAcclY = 0;
int16_t previousAcclX = 0;


// Reserve space for entries in the average bucket.
// Change the type between < and > to change the entire way the library works.
Average<int16_t> avgAcclZ(AVG_READINGS);
bool shouldAverageZ = true;
Average<int16_t> avgAcclY(AVG_READINGS);
bool shouldAverageY = true;
Average<int16_t> avgAcclX(AVG_READINGS);
bool shouldAverageX = true;

void setup() {
    Serial.begin(9600);
    Keyboard.begin();
    Wire.begin();
    // nunchuk_init_power(); // A1 and A2 is power supply
    nunchuk_init();
    // wait for it to init 
    while(!nunchuk_read()){
      delay(100);
    }
    // get averages
    for(uint16_t i = 0; i <= AVG_READINGS*2; i++ ){
      avgAcclX.push(nunchuk_accelX());
      avgAcclY.push(nunchuk_accelY());
      avgAcclZ.push(nunchuk_accelZ());
      delay(POLLING_DELAY);
    }
    // get previous
    previousAcclZ = nunchuk_accelX();
    previousAcclY = nunchuk_accelY();
    previousAcclX = nunchuk_accelZ();
}

void checkJump(){
  int16_t breakpoint = (int16_t) avgAcclY.mean() - 50;
  if(nunchuk_accelY() < breakpoint && !isJumpPressed){
    // first press
    Keyboard.press(' ');
    shouldAverageY = false;
    isJumpPressed = true;
  } else if(nunchuk_accelY() >= breakpoint && isJumpPressed){
    // first release
    Keyboard.release(' ');
    shouldAverageY = true;
    isJumpPressed = false;
  }
}

void checkLean(){
//  left
  if(nunchuk_accelX() >  100 && !isLeanRightPressed){
    // first press
    Keyboard.press('e');
    isLeanRightPressed = true;
  } else if(nunchuk_accelX() <= 100 && isLeanRightPressed){
    // first release
    Keyboard.release('e');
    isLeanRightPressed = false;
  }

    //  right
  if(nunchuk_accelX() <  -100 && !isLeanLeftPressed){
    // first press
    Keyboard.press('q');
    isLeanLeftPressed = true;
  } else if(nunchuk_accelX() >= -100 && isLeanLeftPressed){
    // first release
    Keyboard.release('q');
    isLeanLeftPressed = false;
  }
}

void checkButtonC(){
   // c button
    if(nunchuk_buttonC() == 1 && !isCPressed){
      // first press
      Keyboard.press('r');
      isCPressed = true;
    } else if(nunchuk_buttonC() == 0 && isCPressed){
      // first release
      Keyboard.release('r');
      isCPressed = false;
    }
}

void checkRun(){
    if(nunchuk_joystickY() > 120 && !isShiftPressed){
    // press left
    Keyboard.press(KEY_LEFT_SHIFT);
    isShiftPressed = true;
    if(isUpPressed){
        // need to repress, catch on next cycle
        Keyboard.release('w');
        isUpPressed = false;
    }
  } else if(nunchuk_joystickY() <= 120 && isShiftPressed){
    Keyboard.release(KEY_LEFT_SHIFT);
    isShiftPressed = false;
    if(isUpPressed){
        // need to repress, catch on next cycle
        Keyboard.release('w');
        isUpPressed = false;
    }
  }
}

void checkButtonZ(){
   // c button
    if(nunchuk_buttonZ() == 1 && !isZPressed){
      // first press
      Keyboard.press('f');
      isZPressed = true;
    } else if(nunchuk_buttonZ() == 0 && isZPressed){
      // first release
      Keyboard.release('f');
      isZPressed = false;
    }
}

void checkJoystick(){
  //left
  if(nunchuk_joystickX() < -5 && !isLeftPressed){
    // press left
    Keyboard.press('a');
    isLeftPressed = true;
  } else if(nunchuk_joystickX() >= -5 && isLeftPressed){
    Keyboard.release('a');
    isLeftPressed = false;
  }

  // right 
  if(nunchuk_joystickX() > 5 && !isRightPressed){
    // press left
    Keyboard.press('d');
    isRightPressed = true;
  } else if(nunchuk_joystickX() <= 5 && isRightPressed){
    Keyboard.release('d');
    isRightPressed = false;
  }

  
  // forward
  if(nunchuk_joystickY() > 5 && !isUpPressed){
    // press left
    Keyboard.press('w');
    isUpPressed = true;
  } else if(nunchuk_joystickY() <= 5 && isUpPressed){
    Keyboard.release('w');
    isUpPressed = false;
  }

  // back
  if(nunchuk_joystickY() < -5 && !isDownPressed){
    // press left
    Keyboard.press('s');
    isDownPressed = true;
  } else if(nunchuk_joystickY() >= -5 && isDownPressed){
    Keyboard.release('s');
    isDownPressed = false;
  }
}

void printData(){
  Serial.print("joy: X:");
  Serial.print(nunchuk_joystickX(), DEC);
  Serial.print(", Y:");
  Serial.print(nunchuk_joystickY(), DEC);

  Serial.print("  acc: X:");
  Serial.print(nunchuk_accelX(), DEC);
  Serial.print(", Xavg:");
  Serial.print((int16_t) avgAcclX.mean(), DEC);
  Serial.print(", Y:");
  Serial.print(nunchuk_accelY(), DEC);
  Serial.print(", Yavg:");
  Serial.print((int16_t) avgAcclY.mean(), DEC);
  Serial.print(", Z:");
  Serial.print(nunchuk_accelZ(), DEC);
  Serial.print(", Zavg:");
  Serial.print((int16_t) avgAcclZ.mean(), DEC);

  Serial.print("  but: Z:");
  Serial.print(nunchuk_buttonZ(), DEC);
  Serial.print(", C:");
  Serial.print(nunchuk_buttonC(), DEC);
  Serial.print("\n");
}

void loop() {
    if (nunchuk_read()) {
        // Work with nunchuk_data
        printData();
        checkButtonC();
        checkButtonZ();
        checkJoystick();
        checkJump();
        checkLean();
        checkRun();
        // push averages
        if(shouldAverageX) avgAcclX.push(nunchuk_accelX());
        if(shouldAverageY) avgAcclY.push(nunchuk_accelY());
        if(shouldAverageZ) avgAcclZ.push(nunchuk_accelZ());
        // set previous
        previousAcclZ = nunchuk_accelX();
        previousAcclY = nunchuk_accelY();
        previousAcclX = nunchuk_accelZ();
    } 
    delay(POLLING_DELAY);
}
//
