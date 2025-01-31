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

BlockNot servoSpeedA(20);

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
int posAstep = 5;
bool dirA = true; // true is 0 -> 180
int posB = minPosB;
int posBstep = 10;
bool dirB = true; // true is 0 -> 180

ESP32PWM pwm;

void setup() {
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

  // non blocking servo motion
  if(servoSpeedA.triggered()){
    // increase or decrease position
    if (dirA) { posA += posAstep; } else { posA -= posAstep; }
    // check position and set direction
    if (posA >= maxPosA) { posA = maxPosA; dirA = false; }
    if (posA <= minPosA) { posA = minPosA; dirA = true; }
    // update position of servo'
    if (invertA) { servoA.write(180 - posA); } else { servoA.write(posA); }
  }

}