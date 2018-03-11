#include <Keyboard.h>
#include <Wire.h>
#define NUNCHUK_DEBUG true;
#include "nunchuk.h"

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
//juml;
bool isJumpPressed = false;
int16_t previousAcclZ = 0;
int16_t previousAcclY = 0;
int16_t previousAcclX = 0;
void setup() {

    Serial.begin(9600);
    Keyboard.begin();
    Wire.begin();
    // nunchuk_init_power(); // A1 and A2 is power supply
    nunchuk_init();
    // wait for it to init 
    delay(3000);
    // get previous
    previousAcclZ = nunchuk_accelX();
    previousAcclY = nunchuk_accelY();
    previousAcclX = nunchuk_accelZ();
}

void checkJump(){
  if(nunchuk_accelY() < -50 && !isJumpPressed){
    // first press
    Keyboard.press(' ');
    isJumpPressed = true;
  } else if(nunchuk_accelY() >= -50 && isJumpPressed){
    // first release
    Keyboard.release(' ');
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
    if(nunchuk_joystickY() > 100 && !isShiftPressed){
    // press left
    Keyboard.press(KEY_LEFT_SHIFT);
    isShiftPressed = true;
  } else if(nunchuk_joystickY() <= 100 && isShiftPressed){
    Keyboard.release(KEY_LEFT_SHIFT);
    isShiftPressed = false;
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

void loop() {
    if (nunchuk_read()) {
        // Work with nunchuk_data
        nunchuk_print();
        checkButtonC();
        checkButtonZ();
        checkJoystick();
        checkJump();
        checkLean();
        checkRun();
        // set previous
        previousAcclZ = nunchuk_accelX();
        previousAcclY = nunchuk_accelY();
        previousAcclX = nunchuk_accelZ();
    } 
    delay(100);
}
// 
