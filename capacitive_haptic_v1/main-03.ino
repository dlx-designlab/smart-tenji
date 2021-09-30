
/*connectiob between haptics anf touch using DRVL2605L and Touch board*/

#include <Sparkfun_DRV2605L.h> //SparkFun Haptic Motor Driver Library 

SFE_HMD_DRV2605L HMD; //Create haptic motor driver object

// compiler error handling
#include "Compiler_Errors.h"

// touch includes
#include <MPR121.h>
#include <Wire.h>
#define MPR121_ADDR 0x5C
#define MPR121_INT 4

/* // mp3 includes
#include <SPI.h>
#include <SdFat.h>
#include <FreeStack.h> 
#include <SFEMP3Shield.h>

// mp3 variables
SFEMP3Shield MP3player;
byte result;
int lastPlayed = 0;

// mp3 behaviour defines
#define REPLAY_MODE TRUE  // By default, touching an electrode repeatedly will 
                          // play the track again from the start each time.
                          //
                          // If you set this to FALSE, repeatedly touching an 
                          // electrode will stop the track if it is already 
                          // playing, or play it from the start if it is not. */

// touch behaviour definitions
#define firstPin 0
#define lastPin 11

/* // sd card instantiation
SdFat sd; */

void setup(){  
  HMD.begin();
  Serial.begin(57600);

  HMD.Mode(0); // Internal trigger input mode -- Must use the GO() function to trigger playback.
  HMD.MotorSelect(0x36); // ERM motor, 4x Braking, Medium loop gain, 1.365x back EMF gain
  HMD.Library(2); //1-5 & 7 for ERM motors, 6 for LRA motors 
  
  pinMode(LED_BUILTIN, OUTPUT);
   
  //while (!Serial) ; {} //uncomment when using the serial monitor 
  Serial.println("Bare Conductive Proximity MP3 player");

  // if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt();

  if(!MPR121.begin(MPR121_ADDR)) Serial.println("error setting up MPR121");
  MPR121.setInterruptPin(MPR121_INT);

  // Changes from Touch MP3
  
  // this is the touch threshold - setting it low makes it more like a proximity trigger
  // default value is 40 for touch
  MPR121.setTouchThreshold(8);
  
  // this is the release threshold - must ALWAYS be smaller than the touch threshold
  // default value is 20 for touch
  MPR121.setReleaseThreshold(4);  

  /* result = MP3player.begin();
  MP3player.setVolume(10,10);
 
  if(result != 0) {
    Serial.print("Error code: ");
    Serial.print(result);
    Serial.println(" when trying to start MP3 player");
   } */
   
}

void loop(){
  readTouchInputs();
  //int seq = 0; //There are 8 sequence registers that can queue up to 8 waveforms
}


void readTouchInputs(){
  if(MPR121.touchStatusChanged()){
    
    MPR121.updateTouchData();

    // only make an action if we have one or fewer pins touched
    // ignore multiple touches
    
    if(MPR121.getNumTouches()<=1){
      for (int i=0; i < 12; i++){  // Check which electrodes were pressed
        if(MPR121.isNewTouch(i)){
        
            //pin i was just touched
            Serial.print("pin ");
            Serial.print(i);
            Serial.println(" was just touched");
            digitalWrite(LED_BUILTIN, HIGH);
            //haptics
            HMD.Waveform(1, 52);
            HMD.go();
            delay(600); //give enough time to play effect 
            
            /*if(i<=lastPin && i>=firstPin){
              HMD.Waveform(1, 120);
              HMD.go();
              delay(600); //give enough time to play effect 
            } */    
        }else{
          if(MPR121.isNewRelease(i)){
            Serial.print("pin ");
            Serial.print(i);
            Serial.println(" is no longer being touched");
            digitalWrite(LED_BUILTIN, LOW);
         } 
        }
      }
    }
  }
}
