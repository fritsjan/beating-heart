/*
  HeartBeat with servo's
  by ProductBakery 2025

  Hardware used: Esp32-C3 Mini, MG90S servo's
*/

#include <Arduino.h>
#include <BlockNot.h>
#include <ESP32Servo.h>

#define LED 8
#define servo1Pin 6
#define servo2Pin 7

bool ledState = LOW;

Servo servoA;
Servo servoB;

BlockNot servoSpeed(20);
BlockNot startTimerA(1000);
BlockNot startTimerB(1000);

// Published values for SG90 servos; adjust if needed
int minUs = 500;
int maxUs = 2500;

int minPosA = 0;
int maxPosA = 90;
int minPosB = 0;
int maxPosB = 90;
bool invertA = false;
bool invertB = true;

int posA = minPosA;
int posAstep = 10;
bool dirA = true; // true is 0 -> 180
int posB = minPosB;
int posBstep = 10;
bool dirB = true; // true is 0 -> 180

int servoModeA = 0;
int servoModeB = 0;

ESP32PWM pwm;

bool run = true;

void processSerial() {
    String input = Serial.readString();
    if (input.startsWith("p")) {
        run = !run;
        Serial.println((run ? "Loop RUNNING" : "Loop PAUSED"));
    }
    if (input.startsWith("s")) {
        servoModeA = 1;
        servoModeB = 1;
        Serial.println("StartA");
    }
    while(Serial.available()) Serial.read();
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("\nREADY!\n"));

  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  servoA.setPeriodHertz(50);      // Standard 50hz servo
	servoB.setPeriodHertz(50);      // Standard 50hz servo
  servoA.attach(servo1Pin, minUs, maxUs);
  servoB.attach(servo2Pin, minUs, maxUs);
  pinMode(LED, OUTPUT);

}


void loop() {

  if (Serial.available()) processSerial();

  if(startTimerA.triggered()) { servoModeA = 1;}
  if(startTimerB.triggered()) { servoModeB = 1;}

  if(run){

    if(servoSpeed.triggered()){

      switch(servoModeA){
        case 0:
        // init
          posA = 0;
          //servoModeA = 1;
          break;
        case 1:
          //move from 0 - 90
          posA += posAstep;
          if (posA >= maxPosA) { posA = maxPosA; servoModeA = 2; }
          if (invertA) { servoA.write(180 - posA); } else { servoA.write(posA); }
          break;
        case 2:
          //move from 90 - 0
          posA -= posAstep;
          if (posA <= minPosA) { posA = minPosA; servoModeA = 3;} //Serial.println("go idle");}
          if (invertA) { servoA.write(180 - posA); } else { servoA.write(posA); }
          break;
        case 3: 
          // idle
          break;
      }

      switch(servoModeB){
        case 0:
        // init
          posB = 0;
          //servoModeB = 1;
          break;
        case 1:
          //move from 0 - 90
          posB += posBstep;
          if (posB >= maxPosB) { posB = maxPosB; servoModeB = 2; }
          if (invertB) { servoB.write(180 - posB); } else { servoB.write(posB); }
          break;
        case 2:
          //move from 90 - 0
          posB -= posBstep;
          if (posB <= minPosB) { posB = minPosB; servoModeB = 3;} //Serial.println("go idle");}
          if (invertB) { servoB.write(180 - posB); } else { servoB.write(posB); }
          break;
        case 3: 
          // idle
          break;
      }

    }



    // // non blocking servo motion
    // if(servoSpeed.triggered()){
    //   // increase or decrease position
    //   if (dirA) { posA += posAstep; } else { posA -= posAstep; }
    //   // check position and set direction
    //   if (posA >= maxPosA) { posA = maxPosA; dirA = false;  }
    //   if (posA <= minPosA) { posA = minPosA; dirA = true;   }
    //   // update position of servo
    //   if (invertA) { servoA.write(180 - posA); } else { servoA.write(posA); }
    //   if (invertB) { servoB.write(180 - posA); } else { servoB.write(posA); }
    // }

  }



  // servoSpeed.setDuration(10); // update speed of servo movement

}