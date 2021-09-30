/*connectiob between haptics and touch using motor shield and Touch board*/

// compiler error handling
#include "Compiler_Errors.h"

// touch includes
#include <MPR121.h>
#include <Wire.h>
#define MPR121_ADDR 0x5C
#define MPR121_INT 4

#include <AFMotor.h>

AF_DCMotor left_motor(2, MOTOR12_64KHZ); // create motor #2, 64KHz pwm

#define firstPin 0
#define lastPin 11

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  
  left_motor.setSpeed(255);     // set the speed to 200/255

  //while (!Serial) ; {} //uncomment when using the serial monitor 
  Serial.println("Bare Conductive Proximity MP3 player");

  // if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt();

  if(!MPR121.begin(MPR121_ADDR)) Serial.println("error setting up MPR121");
  MPR121.setInterruptPin(MPR121_INT);

  // this is the touch threshold - setting it low makes it more like a proximity trigger
  // default value is 40 for touch
  MPR121.setTouchThreshold(8);
  
  // this is the release threshold - must ALWAYS be smaller than the touch threshold
  // default value is 20 for touch
  MPR121.setReleaseThreshold(4);  
}

void loop() {

  readTouchInputs();
 
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
            //motor
            left_motor.run(FORWARD);      // turn it on going forward
            //delay(1000);
            
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
            //motor
            left_motor.run(RELEASE);      // stopped
         } 
        }
      }
    }
  }
}

