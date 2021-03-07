#include <Joystick.h>

//Erik's Box

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_MULTI_AXIS,
  14, 0,                  // Button Count, Hat Switch Count
  true, true, true,     // X and Y, but no Z Axis
  true, true, true,     // No Rx, Ry, or Rz
  false, true,          // No rudder or throttle
  false , false, false);  // No accelerator, brake, or steering

  //note - I changed the library to have 2 throttles, and forced all other "simulation" flags to false(they seem to be broken anyway)

const int sizeOfDigitalInputs = 8; //toggles 8. flash on/off, we can do 1 switch, so 2 inputs. 8 + 2 = 10; 
const int sizeOfAnalogInputs = 8;
//int digitalInputs[sizeOfDigitalInputs] = {0,1,2,3,5,7,14,15};
//wiring not in order so use this so toggles match up with button numbers
int digitalInputs[sizeOfDigitalInputs] = {0,14,15,3,2,1,5,7};
int analogInputs[sizeOfAnalogInputs] = {  A0,A1,A2,A10,A9,A8,A7,A6};//NOTE, to read analog we need to use A prefix//20 and 21 can be used for digital

bool printDebug = false;

void setup() {
  // put your setup code here, to run once:

  if(printDebug)
    Serial.begin(9600);

  //SetInputsTest();
  SetInputs();

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
  
  Joystick.begin();
}


void loop() 
{
  // put your main code here, to run repeatedly:

  //buttons 0 - 7
  ToggleSwitches();
  //buttons 8 - 14
  MomentarySwitches();
  //
  Dials();
  
  //Debug();

 //send joystick up the pip
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
  //analog to digital conversion
  //read as analog
    pinMode(A3, INPUT);
  
  
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

void ToggleSwitches()
{
  for(int i = 0; i < 8; i++)
  {    
      Joystick.setButton(i, digitalRead(digitalInputs[i]));
  }
}

void MomentarySwitches()
{
  //AnalogToDigitalValues
  //Will make these switches only able to be pressed one at a time ( return to centre when not pushed)
  //Possible to make code detect two but chances of an error is increased whereas Erik will probably never try and press both buttons at the same time.
  
  //deduct which buttons are being pressed by analog value
  if(printDebug)
    Serial.println(analogRead(A3));

  //switch 1 is bottom right (has lowest value)
  //340 - right position
  //421 - left position

  //switch 2 is top right
  //732 right position
  //853 left

  //switch 3 is left switch
  //639 is lower postion
  //981 is upper

  //create ranges with as large an error window as possible (half way between input values)
  //values in order (and half values)
  
  //340
    //  381
  //421
    //  530
  //639
    //  687
  //732
    //  793
  //853
    //  917
  //981
  
  int v = analogRead(A3);
  
  //switch practice for switch!
  //momentary switches buttons on 9-15
  // switch numbers don't make a lot of sense because my wiring doesn't make a lot of sense

  
            
   switch (v)  
        {           
            case 1 ... 381: 
            if(printDebug) Serial.println(" Switch 1 - Right");
            //set joystick button
            Joystick.setButton(13, HIGH);            
            break;

            case 382 ... 530: 
            if(printDebug) Serial.println(" Switch 1 - Left"); 
            //set joystick button
            Joystick.setButton(12, HIGH);
            break;

            case 531 ... 687: 
            if(printDebug) Serial.println(" Switch 3 - Down"); 
            //set joystick button
            Joystick.setButton(8, HIGH);
            break;

            case 688 ... 793: 
            if(printDebug) Serial.println(" Switch 2 - Right"); 
            //set joystick button
            Joystick.setButton(11, HIGH);
            break;

            case 794 ... 917: 
            if(printDebug) Serial.println(" Switch 2 - Left"); 
            //set joystick button
            Joystick.setButton(10, HIGH);
            break;

            case 918 ... 1025: 
            if(printDebug) Serial.println(" Switch 3 - Up"); 
            //set joystick button
            Joystick.setButton(9, HIGH);
            break;
            
            
            default: 
            if(printDebug) Serial.println("No press"); 
            //reset all
            for (int i = 8; i < 14; i++)
            {
              Joystick.setButton(i, LOW);
            }
            break; 
        } 
}



void Debug()
{
Serial.println("Digital");
  
  for (int i = 0; i < sizeOfDigitalInputs; i++)
  {

    
    int v = 0;
    if (digitalRead(digitalInputs[i]) == LOW) 
    {   
      v = 1;
    } 
    
    Serial.print(digitalInputs[i]);
    Serial.print("\t");
    Serial.println(v);

    
  }
  
  Serial.println("Analog");

  for (int i = 0; i < sizeOfAnalogInputs; i++)
  {

    Serial.print(analogInputs[i]);
    Serial.print("\t");
    Serial.println(analogRead(analogInputs[i]));

  }

  Serial.println("Flash");

  
    
    Serial.print("\t");
    Serial.println(analogRead(A3));

  
  
  
  Serial.println("End");
  delay(50);
}
