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
BlockNot startTimerA(1200);
BlockNot startTimerB(100); // time between A and B
BlockNot changeBeat(10000);

// Published values for SG90 servos; adjust if needed
int minUs = 500;
int maxUs = 2500;

int minPosA = 0;
int minPosB = 0;
int maxPosA = 100;
int maxPosB = 100;
bool invertA = false;
bool invertB = true;

int posA = minPosA;
int posB = minPosB;
int posAstep = 10;
int posBstep = 10;
bool dirA = true; // true is 0 -> 180
bool dirB = true; // true is 0 -> 180

int servoModeA = 0;
int servoModeB = 0;

ESP32PWM pwm;

bool run = true;
bool beat = true; // switch between 2 different beats

void processSerial() {
    String input = Serial.readString();
    if (input.startsWith("p")) {
        run = !run;
        Serial.println((run ? "Loop RUNNING" : "Loop PAUSED"));
    }
    if (input.startsWith("q")) {
        maxPosA = 160;
        maxPosB = 160;
        Serial.println("larger");
    }
    if (input.startsWith("w")) {
        maxPosA = 90;
        maxPosB = 90;
        Serial.println("normal");
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

  startTimerB.stop();

}


void loop() {

  if (Serial.available()) processSerial();

  if(run){

    if(startTimerA.triggered()) { 
      startTimerB.start(WITH_RESET); // reset and start delay for timer B
      servoModeA = 10; // start servoA
      }
    if(startTimerB.firstTrigger()) { 
      servoModeB = 10; // start servoB
      }
    
    if(changeBeat.triggered()){
      beat = !beat;
      if (beat){
        maxPosA = 100;
        maxPosB = 100;
      } else {
        maxPosA = 140;
        maxPosB = 140;
      }
    }

    if(servoSpeed.triggered()){ // update on servoSpeed timer fired

      switch(servoModeA){
        case 0:
          // idle
          break;
        case 10:
          // start
          posA = 0;
          // go move
          servoModeA = 20;
          break;
        case 20:
          //move from 0 - 90
          posA += posAstep;
          if (posA >= maxPosA) { posA = maxPosA; servoModeA = 30; } // go back
          if (invertA) { servoA.write(180 - posA); } else { servoA.write(posA); }
          break;
        case 30:
          //move from 90 - 0
          posA -= posAstep;
          if (posA <= minPosA) { posA = minPosA; servoModeA = 0;} //go idle
          if (invertA) { servoA.write(180 - posA); } else { servoA.write(posA); }
          break;

      }

      switch(servoModeB){
        case 0:
          // idle
          break;
        case 10:
          //init
          posB = 0;
          servoModeB = 20;
          break;
        case 20:
          //move from 0 - 90
          posB += posBstep;
          if (posB >= maxPosB) { posB = maxPosB; servoModeB = 30; } // go back
          if (invertB) { servoB.write(180 - posB); } else { servoB.write(posB); }
          break;
        case 30:
          //move from 90 - 0
          posB -= posBstep;
          if (posB <= minPosB) { posB = minPosB; servoModeB = 0;} // go idle
          if (invertB) { servoB.write(180 - posB); } else { servoB.write(posB); }
          break;
      }

    }

  } else {

      int wait = 2000;

      servoA.write(90);
      delay(wait);
      servoA.write(180);
      delay(wait);
      servoA.write(90);
      delay(wait);
      servoA.write(0);
      delay(wait);
  }

  // servoSpeed.setDuration(10); // update speed of servo movement

}