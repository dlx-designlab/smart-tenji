/*connection between haptics and touch using motor shield and Touch board*/

// compiler error handling
#include "Compiler_Errors.h"

// touch includes
#include <MPR121.h>
#include <Wire.h>
#define MPR121_ADDR 0x5C
#define MPR121_INT 4

#include <Adafruit_MotorShield.h> //adafruit motor sheild v2.3

// Create the motor shield object with the default I2C address
// Adafruit_MotorShield AFMS1 = Adafruit_MotorShield(0x60);
// Adafruit_MotorShield AFMS2 = Adafruit_MotorShield(0x61);

Adafruit_MotorShield MotorShields[2] = {Adafruit_MotorShield(0x60), Adafruit_MotorShield(0x61)}; //creat an array of motor sheilds

// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Adafruit_DCMotor *myMotor1 = MotorShields[0].getMotor(1); //create motor at port 1
// Adafruit_DCMotor *myMotor2 = MotorShields[0].getMotor(2); //create motor at port 2
// Adafruit_DCMotor *myMotor3 = MotorShields[1].getMotor(1); //create motor at port 3
// Adafruit_DCMotor *myMotor4 = MotorShields[1].getMotor(2); //create motor at port 4

Adafruit_DCMotor *motors[2][2] = {        //creat an array of motors in the motor shield
  {MotorShields[0].getMotor(1), MotorShields[0].getMotor(2)},
  {MotorShields[1].getMotor(1), MotorShields[1].getMotor(2)}
};


#define firstPin 0
#define lastPin 11

int posX;                     // position x
int posY;                     // position y

const int floor_w = 2;        // floor width
const int floor_h = 2;        // floor height


int tenji_matrix[floor_w][floor_h] = { //user position matrix
  {0,0},
  {0,0}
  }; 

int tenji_motors[floor_w][floor_h] = { //morot array 
  {0,0}, //{motor1,motor2},
  {0,0}  //{motor3,motor4}
  }; 

int animation_array_01[2][floor_w][floor_h] = {  //animation 1 swipe down
  {
    {1,1},
    {0,0}
  },
  {
    {0,0},
    {1,1}
  }
}; 

int animation_array_02[2][floor_w][floor_h] = { //animation 2 swipe to right
  {
    {1,0},
    {1,0}
  },
  {
    {0,1},
    {0,1}
  }
}; 

int animation_array_03[2][floor_w][floor_h] = { //animation 3 swipe to left
  {
    {0,1},
    {0,1}
  },
  {
    {1,0},
    {1,0}
  }
};

int animation_array_04[2][floor_w][floor_h] = {  //animation 4 swipe up
  {
    {0,0},
    {1,1}
  },
  {
    {1,1},
    {0,0}
  }
}; 

void setup() {

  Serial.begin(9600);          // set up Serial library at 9600 bps

  if (!MotorShields[0].begin() || !MotorShields[1].begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }

  Serial.println("Motor Shield found.");

  //set DC motor speed 0 (off) -> 255 (max speed)
  
  motors[0][0]->setSpeed(150); 
  motors[1][0]->setSpeed(150); 
  motors[1][1]->setSpeed(150); 
  motors[0][1]->setSpeed(150); 

  //while (!Serial) ; {} //uncomment when using the serial monitor 
  //Serial.println("Bare Conductive Proximity MP3 player");

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

  // motor_animations(animation_array_01[0]);
  // delay(500);
  // motor_animations(animation_array_01[1]);
  // delay(500);
  // motor_animations(animation_array_01[2]);
  // delay(1000);

}

void readTouchInputs(){

  if(MPR121.touchStatusChanged()){
    
    MPR121.updateTouchData();

    // only make an action if we have one or fewer pins touched
    // ignore multiple touches
    
    if(MPR121.getNumTouches()<=1){
      for (int i=0; i < 4; i++){  // Check which electrodes were touched
        if(MPR121.isNewTouch(i)){
        
            //****pin i was just touched****

            //calculate user position
            posX = i % floor_w;
            posY = i / floor_h;

            tenji_matrix[posX][posY] = 1; //input 1 into matrix in position X and Y

            //print data to serial

            Serial.print("pin ");
            Serial.print(i);
            Serial.println(" was just touched");

            digitalWrite(LED_BUILTIN, HIGH);

            Serial.println("X: " + String(posX) + ", Y: " + String(posY)); //print position
            //Serial.print("\r");
            //Serial.print(tenji_matrix[floor_w][floor_h]);

            for ( int j = 0; j < floor_h; j++ ) {
              // loop through columns of current row
              for ( int k = 0; k < floor_w; k++ ){
                Serial.print (tenji_matrix[ j ][ k ] );
              }
              Serial.println(" "); // start new line of output
            }

            // activate motor

            motors[0][0]->run(FORWARD);      // turn motor on going forward
            motors[1][0]->run(FORWARD); 
            //delay(1000);
              
        }else{
          if(MPR121.isNewRelease(i)){
            Serial.print("pin ");
            Serial.print(i);
            Serial.println(" is no longer being touched");
            digitalWrite(LED_BUILTIN, LOW);

            posX = i % floor_w;
            posY = i / floor_h;
            tenji_matrix[posX][posY] = 0; //reset matrix

            //motor
            motors[0][0]->run(RELEASE);      // motor stopped
            motors[1][0]->run(RELEASE);
         } 
        }
      }
    }
  }
}

// void motor_animations(int motors_array[floor_w][floor_h]) {

//   int motor_on;

//   for (int j = 0; j < floor_h; j++) {
//     // loop through columns of current row
//     for (int k = 0; k < floor_w; k++){
//      if (animation_array_01[j][k] == 1) {
//        tenji_motors[j][k] == 1;
//        motor_on = 1;
//       }
//     }
//   }



//}




