/*******************************************************************************
 *
 *	Solar powered security lighting controller
 *	Course: ECE4900
 *	Author: Matthew Young
 *	Last Modified: 20170413
 *
 ******************************************************************************/
 
#include <Adafruit_SleepyDog.h>
#include <Timer.h>

// initialize global constants
const int solarPin = A0;
const int pirPin = 2;       // pin2 is an interrupt pin.
const int HPLedPin = 13;
const int LPLedPin = 12;
const int cron = 8000;      // 8sec
const int HPLedLen = 30000; // 30sec

//initialize global vars
int intensity = 0;
int nightIntensity = 400;
int pirValue = 0;
int id = -1;
bool isNight = false;
bool isrAttached = false;
Timer t;

void setup() {
  // setup pins
  pinMode(HPLedPin, OUTPUT);
  pinMode(HPLedPin, OUTPUT);
  pinMode(pirPin, INPUT_PULLUP);
  digitalWrite(HPLedPin, LOW);
  digitalWrite(HPLedPin, LOW);
  
  // initialize timer function
  t.every(cron, checkSolar);
}

// ISR for PIR sensor
void motion() {
  t.stop(id);
  id = t.pulse(HPLedPin, HPLedLen, HIGH);
}

// Function executed by timer every sleeplen ms
void checkSolar() {
  intensity = analogRead(solarPin);
  bool wasNight = isNight;
  isNight = intensity <= nightIntensity;
  if (!(isNight && wasNight)) {
    digitalWrite(LPLedPin, isNight);
    if (isNight) {
      attachInterrupt(0, motion, RISING);
    } else {
      detachInterrupt(0);
    }
  }
}

void loop() {
  t.update();
  Watchdog.sleep();
}
