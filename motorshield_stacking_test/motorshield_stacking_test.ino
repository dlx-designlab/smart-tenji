/*
stacking test for capacitive tenji 2021
*/

#include <Adafruit_MotorShield.h>

#include <MPR121.h>
#include <Wire.h>
#define MPR121_ADDR 0x5C
#define MPR121_INT 4

Adafruit_MotorShield AFMSbot = Adafruit_MotorShield(0x60); // Rightmost jumper closed
Adafruit_MotorShield AFMStop = Adafruit_MotorShield(0x61);; // Default address, no jumpers

// And a DC Motor to port M1
Adafruit_DCMotor *myMotor1 = AFMSbot.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMSbot.getMotor(2);

Adafruit_DCMotor *myMotor3 = AFMStop.getMotor(1);
Adafruit_DCMotor *myMotor4 = AFMStop.getMotor(2);

void setup() {
  while (!Serial);
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("MMMMotor party!");

  AFMSbot.begin(); // Start the bottom shield
  AFMStop.begin(); // Start the top shield

  // turn on the DC motor
  myMotor1->setSpeed(200);
  myMotor1->run(RELEASE);

  myMotor2->setSpeed(200);
  myMotor2->run(RELEASE);

  myMotor3->setSpeed(200);
  myMotor3->run(RELEASE);

  myMotor4->setSpeed(200);
  myMotor4->run(RELEASE);
}

void loop() {

  myMotor1->run(FORWARD);
  delay(1000);
  myMotor1->run(RELEASE);  
  myMotor2->run(FORWARD);
  delay(1000);
  myMotor2->run(RELEASE);  
  myMotor3->run(FORWARD);
  delay(1000);
  myMotor3->run(RELEASE);  
  myMotor4->run(FORWARD);
  delay(1000);
  myMotor4->run(RELEASE);   

}