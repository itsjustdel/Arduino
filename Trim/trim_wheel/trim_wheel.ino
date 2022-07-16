#include <Joystick.h>
#define CLK 4
#define DT 7
//#define SW 7

int sensitivity = 2;
int xPos;
int yPos;
int zPos;

int switchPos = 0;
int axisRange = 512; // one way

int counter = 0;
int currentStateCLK;
int lastStateCLK;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   0, 0,                  // Button Count, Hat Switch Count
                   true, true, true,     // X and Y, but no Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

void setup() {
  // rotary
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  // switch
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);

  // Initialize Joystick Library
  Joystick.begin();

  Joystick.setXAxisRange(-axisRange, axisRange);
  Joystick.setYAxisRange(-axisRange, axisRange);
  Joystick.setZAxisRange(-axisRange, axisRange);


  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);
}

void loop() {
  rotary();
  switch3Way();
}

void switch3Way() {
  int a = digitalRead(15);
  int b = digitalRead(16);

  if (a == 1 && b == 1) {
    switchPos = 1;
  } else if (b == 1) {
    switchPos = 2;
  } else {
    switchPos = 0;
  }

}

void setAxis(int counter) {

  counter *= sensitivity;
  if (switchPos == 0) {
    xPos += counter;
    if(xPos > axisRange){
      xPos = axisRange;
    } else if (xPos < -axisRange){
      xPos = -axisRange;
    }
    Joystick.setXAxis(xPos);
//    Serial.print("x = ");
//    Serial.println(xPos);

  } else if (switchPos == 1) {
    yPos += counter;
     if(yPos > axisRange){
      yPos = axisRange;
    } else if (yPos < -axisRange){
      yPos = -axisRange;
    }
    Joystick.setYAxis(yPos);
//    Serial.print("y = ");
//    Serial.println(yPos);
  } else {
    zPos += counter;
     if(zPos > axisRange){
      zPos = axisRange;
    } else if (zPos < -axisRange){
      zPos = -axisRange;
    }
    Joystick.setZAxis(zPos);
//    Serial.print("z = ");
//    Serial.println(zPos);
  }
}

void rotary() {

  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      if (counter > -axisRange) {
        counter -= sensitivity;
      }
      setAxis(-1);
    } else {
      // Encoder is rotating CW so increment
      setAxis(1);
    }
  }
  // Remember last CLK state
  lastStateCLK = currentStateCLK;
}
