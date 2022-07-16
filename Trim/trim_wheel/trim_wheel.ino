#include <Joystick.h>

#define outputA 4
#define outputB 7

int counter = 0;
int aState;
int aLastState;
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   1, 0,                  // Button Count, Hat Switch Count
                   true, true, true,     // X and Y, but no Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

void setup() {
  // Initialize Button Pins
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

  // Initialize Joystick Library
  Joystick.begin();

  Joystick.setXAxisRange(-512, 512);//can go 1024
  Joystick.setYAxisRange(-128, 128);
  Joystick.setZAxisRange(-128, 128);

  Joystick.setButton(0, 1);
}

// Last state of the buttons
int lastButtonState[5] = {0, 0, 0, 0, 0};

void loop() {

  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState) {
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != aState) {
      counter += 2;
      if (counter > 512) {
        counter = 512;        
      }      
      Joystick.setXAxis(-counter);
    } else {
      counter -= 2;
      if (counter < -512) {
        counter = -512;        
      }
      Joystick.setButton(0, 0);
      Joystick.setXAxis(-counter);
    }
//    Serial.print("Position: ");
//    Serial.println(counter);
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state

}
