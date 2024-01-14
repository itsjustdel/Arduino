//Namco Stick rewire

#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  10, 1,                  // Button Count, Hat Switch Count
  false, false, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering


int digitalInputs[10] = {2,3,4,5,6,7,16,10,A0,A3};

//a1 - down
//a2 - left
//15 - right
//14 - up/jump

int hatInputs[4] = {A1,15,A2,14};
int hatValues[4];
void setup() {
  // Initialize Button Pins
  SetInputs();
  Joystick.begin();
 
}

void loop() 
{
    Buttons();
    Hat();
}

void SetInputs()
{
    // Initialize Button Pins -just do all as digital
  for(int i = 0; i < 22; i++)
  {  
    pinMode(i, INPUT_PULLUP);
  }
}
void Hat()
{
  
  for(int i = 0; i < 4; i++)
  {
    int currentButtonState = !digitalRead(hatInputs[i]);
    hatValues[i] = !digitalRead(hatInputs[i]);
  }

//set hat direction in degrees - 0 for 1st hat switch
  //Wires not in great order

  //reset
  //-1 means off
  if(hatValues[0] == LOW && hatValues[1] == LOW &&hatValues[2] == LOW &&hatValues[3] == LOW)
    Joystick.setHatSwitch(0,-1);

  //look for diagonals
  if(hatValues[3] == HIGH && hatValues[1] == HIGH)
  {
    Joystick.setHatSwitch(0,45);
  }
  else if(hatValues[1] == HIGH && hatValues[0] == HIGH)
  {
    Joystick.setHatSwitch(0,135);
  }
  else if(hatValues[0] == HIGH && hatValues[2] == HIGH)
  {
    Joystick.setHatSwitch(0,225);
  }
  else if(hatValues[2] == HIGH && hatValues[3] == HIGH)
  {
    Joystick.setHatSwitch(0,315);
  }    
  else if(hatValues[1] == HIGH)
    Joystick.setHatSwitch(0,90);

  else if(hatValues[0] == HIGH)
    Joystick.setHatSwitch(0,180);

  else if(hatValues[2] == HIGH)
    Joystick.setHatSwitch(0,270);

  else if(hatValues[3] == HIGH)
    Joystick.setHatSwitch(0,0);

    //Joystick.setHatSwitch(0,45);

}
void Buttons()
{
  for(int i = 0; i < 10; i++)
  {
    int currentButtonState = !digitalRead(digitalInputs[i]);
    Joystick.setButton(i,currentButtonState);
  }   
}
