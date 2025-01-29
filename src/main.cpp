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

Servo servo1;
Servo servo2;

BlockNot servoTimer1(200);

// Published values for SG90 servos; adjust if needed
int minUs = 500;
int maxUs = 2500;
int pos = 0;      // position in degrees

int minPosA = 0;
int maxPosA = 100;
int minPosB = 90;
int maxPosB = 180;

int posA = 0;
int posAstep = 36;
bool dirA = true; // true is 0 -> 180

ESP32PWM pwm;

void setup() {
  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  servo1.setPeriodHertz(50);      // Standard 50hz servo
	servo2.setPeriodHertz(50);      // Standard 50hz servo
  servo1.attach(servo1Pin, minUs, maxUs);
  servo2.attach(servo2Pin, minUs, maxUs);
  pinMode(LED, OUTPUT);
}


void loop() {


  // // SET POS TEST
  // printf("0 degree\n");
  // servo1.write(0);
  // servo2.write(180);
  // delay(1000);
  // printf("90 degree\n");
  // servo1.write(90);
  // servo2.write(0);
  // delay(1000);
  // printf("180 degree\n");
  // servo1.write(180);
  // servo2.write(90);
  // delay(1000);

  // servo2.write(180); // set servo2 same as 1
  // delay(500);

  // // SWEEP TEST
	// for (pos = 180; pos >= 0; pos -= 5) { // goes from 180 degrees to 0 degrees
	// 	servo1.write(pos);    // tell servo to go to position in variable 'pos'
  //   servo2.write(pos);    // tell servo to go to position in variable 'pos'
	// 	delay(30);             // waits 30ms for the servo to reach the position
  //   printf("pos: %i\n", pos);
	// }
  // for (pos = 0; pos <= 180; pos += 5) { // goes from 0 degrees to 180 degrees
	// 	servo1.write(pos);    // tell servo to go to position in variable 'pos'
  //   servo2.write(pos);    // tell servo to go to position in variable 'pos'
	// 	delay(30);             // waits 30ms for the servo to reach the position
  //   printf("pos: %i\n", pos);
	// }
  // delay(1000);

  // // LED TEST (GPIO 8)
  // ledState = !ledState;
  // digitalWrite(LED, ledState);

  // non blocking servo motion
  if(servoTimer1.triggered()){
    if (dirA) { posA += posAstep; } else { posA -= posAstep; }
    if (posA >= 180)  { posA = 180; dirA = false; }
    if (posA <= 0)    { posA = 0; dirA = true; }
    servo1.write(posA);
  }

}