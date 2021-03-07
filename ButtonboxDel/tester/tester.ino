
//Base example
//**************************************************************//
//  Name    : shiftIn Example 2.1                               //
//  Author  : Carlyn Maw                                        //
//  Date    : 25 Jan, 2007                                      //
//  Version : 1.0                                               //
//  Notes   : Code for using a CD4021B Shift Register       //
//          :                                                   //
//****************************************************************
#include <Joystick.h>
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_MULTI_AXIS,
  24, 0,                  // Button Count, Hat Switch Count
  true, true, true,     // X and Y, but no Z Axis
  true, true, true,     // No Rx, Ry, or Rz
  false, true,          // No rudder or throttle
  false , false, false);  // No accelerator, brake, or steering

  //note - I changed the library to have 2 throttles, and forced all other "simulation" flags to false(they seem to be broken anyway)

const int sizeOfDigitalInputs = 6; 
const int sizeOfAnalogInputs = 8;
int digitalInputs[sizeOfDigitalInputs] = {0,1,21,15,16,14};//,3,2,1,5,7};
int analogInputs[sizeOfAnalogInputs] = {  A0,A1,A2,A10,A9,A8,A7,A6};//NOTE, to read analog we need to use A prefix//20 and 21 can be used for digital

int digitalOutputs[24];
  //button 1 up is 2, down is 3
  //2 is 4, 14
  //3 is 1, 5
  //4 is switch
  //5 is switch
  //6 is switch
  //7 is 16,18
  //8 is 6,7
  //engine
  //9 is 12
  //bottom row
  //10 is 11
  //11 is 9
  //12 is 10
  //13 is 15
  //14 is 13
  //15 is 18
  //16 is 17
  int digitalOutputsOrganised[24] = {2,3,4,14,1,5,99,99,99,99,99,99,16,8,6,7,12,11,9,10,15,13,18,17};


//define where your pins are
int latchPin = 7;
int dataPin = 3;
int clockPin = 2;

//Define variables to hold the data
//for each shift register.
//starting with non-zero numbers can help
//troubleshoot
byte switchVar1 = 72;  //01001000
byte switchVar2 = 159; //10011111
byte switchVar3 = 64; //10 //only using 2

char note2sing[] = {

  'C', 'd', 'e', 'f', 'g', 'a', 'b', 'c'};

void setup() {

  //start serial

  Serial.begin(9600);

  SetInputs();

  //define pin modes

  pinMode(latchPin, OUTPUT);

  pinMode(clockPin, OUTPUT);

  pinMode(dataPin, INPUT);

 Joystick.begin();

}

void SetInputs()
{
  //digital
  for (int i = 0; i < sizeOfDigitalInputs;i++)
  {
    pinMode(digitalInputs[i], INPUT_PULLUP);
  }  
  //analog
  for (int i = 0; i < sizeOfAnalogInputs;i++)
  {
    pinMode(analogInputs[i], INPUT);
  }  

   //Initialise Joystick library
  Joystick.setXAxisRange(0, 255);
  Joystick.setYAxisRange(0, 255);
  Joystick.setZAxisRange(0, 255);
  Joystick.setRxAxisRange(0, 255);
  Joystick.setRyAxisRange(0, 255);
  Joystick.setRzAxisRange(0, 255);
  
  Joystick.setThrottleRange(0, 255);
  Joystick.setRudderRange(0, 255);
  Joystick.setAcceleratorRange(0, 255);
}

void loop() 
{

 //Shift Registers collect input for many buttons
  ShiftRegisters();

  //set array
  ShiftRegistersToArray();
  //re align so buttons are in a nice order for windows usb settings
  ShiftRegisterArrayToJoystick();
  
  Dials();

  //Debug();
}

//------------------------------------------------end main loop
void Debug()
{
   bool debugP2S = true;
 if(debugP2S)
 {
  Serial.println(switchVar1, BIN);

  Serial.println(switchVar2, BIN);
  
  Serial.println(switchVar3, BIN);
 } 
 
 bool debugDials = true;
 if(debugDials)
 {
  Serial.println("Analog");

  for (int i = 0; i < sizeOfAnalogInputs; i++)
  {
  
    Serial.print(analogInputs[i]);
    Serial.print("\t");
    Serial.println(analogRead(analogInputs[i]));
  
  }
 }

  bool debugToggles = true;
  if(debugToggles)
  {
    Serial.println("Digital");
  
    for (int i = 0; i < sizeOfDigitalInputs; i++)
    {
  
      
      int v = 0;
      if (digitalRead(digitalInputs[i]) == LOW) 
      {   
        v = 1;
      } 
    //  Serial.print(digitalInputs[i]);
      Serial.print("\t");
      Serial.println(v);
    }
  }

  //white space
  Serial.println("-------------------");
  //delay so all these print satements can keep up.
  //delay(500);
}

void ShiftRegisters()
{
   //Pulse the latch pin:

  //set it to 1 to collect parallel data

  digitalWrite(latchPin,1);

  //set it to 1 to collect parallel data, wait

  //delayMicroseconds(1);//problems? use this

  //set it to 0 to transmit data serially

  digitalWrite(latchPin,0);

  //while the shift register is in serial mode
  //collect each shift register into a byte
  //the register attached to the chip comes in first

  switchVar1 = shiftIn(dataPin, clockPin);
  switchVar2 = shiftIn(dataPin, clockPin);
  switchVar3 = shiftIn(dataPin, clockPin);
}

void Dials()
{
  //potentiometers
  int val = 0;
  for(int i = 0; i < 8; i ++)
  {
    val = analogRead(analogInputs[i]);
    val = map(val, 0, 1023, 0, 255);  
    
    switch (i)  
    {           
      case 0: 
        Joystick.setZAxis(val);
        
      break;

      case 1: 
        Joystick.setYAxis(val);
       
      break;

      case 2: 
        Joystick.setXAxis(val);
       //Joystick.setThrottle1(val); 
      break;

      case 3: 
        Joystick.setRxAxis(val);              
      break;

      case 4: 
        Joystick.setRyAxis(val);              
      break;

      case 5: 
        Joystick.setRzAxis(val);              
      break;

      case 6: 
        Joystick.setThrottle1(val);              
      break;

      case 7: 
        Joystick.setThrottle(val);              
      break;
    }
  }
}

void ShiftRegisterArrayToJoystick()
{
  for(int i = 0; i < 24; i++)
  {
    int buttonInWindows = i;
    int buttonOnBoard = digitalOutputsOrganised[i] - 1;
    int buttonValue = !digitalOutputs[buttonOnBoard];  
    if(buttonInWindows >= 17)
      buttonValue = !buttonValue;
      
    Joystick.setButton(buttonInWindows,buttonValue);
  }
/*
  Joystick.setButton(0,digitalOutputs[1] );
  Joystick.setButton(1,digitalOutputs[2] );
  Joystick.setButton(2,digitalOutputs[4-1] );
  Joystick.setButton(3,digitalOutputs[14-1] );
*/

 //button 1 up is 2, down is 3
  //2 is 4, 14
  //3 is 1, 5
  //4 is switch
  //5 is switch
  //6 is switch
  //7 is 16,18
  //8 is 6,7
  //engine
  //9 is 12
  //bottom row
  //10 is 11
  //11 is 9
  //12 is 10
  //13 is 15
  //14 is 13
  //15 is 18
  //16 is 17
  //int digitalOutputsOrganised[24] = {2,3,4,14,1,5,99,99,99,99,99,99,16,5,6,7,12,11,9,10,15,13,18,17};
  
}

void ShiftRegistersToArray()
{

  //3 shift registers
  for(int a = 0 ; a <3; a++)
  { 
          
    byte switchVar = switchVar1;//should put in Array    
    int startingByte = 0;
    int registerLength = 8 ;
    if(a ==1)
    {
      //choose register
      switchVar = switchVar2;
    }
    else if(a ==2) 
    {      
      switchVar = switchVar3;
      //last chip only has two switches attached
      startingByte = 0;
      registerLength = 2;
    }
    
    for (int n=startingByte; n<registerLength; n++)
    {    
      //so, when n is 3, it compares the bits
    
      //in switchVar1 and the binary number 00001000
    
      //which will only return true if there is a
    
      //1 in that bit (ie that pin) from the shift
    
      //register.
    
      if (switchVar & (1 << n) )
      {
        //place in correct array entry depending on where we are in the loop going through the bits/bytes
          digitalOutputs[n  + a*8] = 0;  
      }
      else
      {      
          digitalOutputs[n  + a*8] = 1;
      }
    }
  }
}

////// ----------------------------------------shiftIn function
///// just needs the location of the data pin and the clock pin
///// it returns a byte with each bit in the byte corresponding
///// to a pin on the shift register. leftBit 7 = Pin 7 / Bit 0= Pin 0

byte shiftIn(int myDataPin, int myClockPin) {

  int i;

  int temp = 0;

  int pinState;

  byte myDataIn = 0;

  pinMode(myClockPin, OUTPUT);

  pinMode(myDataPin, INPUT);

//we will be holding the clock pin high 8 times (0,..,7) at the
//end of each time through the for loop

//at the begining of each loop when we set the clock low, it will
//be doing the necessary low to high drop to cause the shift
//register's DataPin to change state based on the value
//of the next bit in its serial information flow.
//The register transmits the information about the pins from pin 7 to pin 0
//so that is why our function counts down

  for (i=7; i>=0; i--)
  {

    digitalWrite(myClockPin, 0);

    delayMicroseconds(2);

    temp = digitalRead(myDataPin);

    if (temp) 
    {

      pinState = 1;

      //set the bit to 0 no matter what

      myDataIn = myDataIn | (1 << i);

    }
    else 
    {

      //turn it off -- only necessary for debuging

     //print statement since myDataIn starts as 0

      pinState = 0;

    }

    //Debuging print statements

    //Serial.print(pinState);

    //Serial.print("     ");

    //Serial.println (dataIn, BIN);

    digitalWrite(myClockPin, 1);

    }

  //debuging print statements whitespace

  //Serial.println();

  //Serial.println(myDataIn, BIN);

  return myDataIn;
}
