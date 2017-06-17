#include "SevenSeg.h"
#include "Adafruit_TLC5947.h"

// How many boards do you have chained?
#define NUM_TLC5974 1

//#define data   46
//#define clock   45
//#define latch   47
#define data   5
#define clock   4
#define latch   6
#define numDisplays 2

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);

SevenSeg scoreDisplay(1, 0, 4095); // num digits, num pins offset, max brightness
SevenSeg timerDisplay(1, 8, 2048); // num digits, num pins offset, max brightness

SevenSeg *displays[numDisplays] = {&scoreDisplay, &timerDisplay};


int counter = 0;
int score = 0;

unsigned long currentTime;
unsigned long lastCounted;
unsigned long lastUpdated;

void setup() {
  Serial.begin(9600);
  Serial.println("Setting up...");

  tlc.begin();
  
  Serial.println("Ready!");

  scoreDisplay.set(2);
  timerDisplay.set(4);

  timerDisplay.pinStates[2] = 1000;
  scoreDisplay.pinStates[3] = 1000;

  updateDisplays();
  delay(200);
}

void loop() {
  currentTime = millis();

//  if (currentTime - lastCounted >= 1000) {
//    counter++;
//    Serial.println(counter);
//    scoreDisplay.set(counter);
//    timerDisplay.set(counter);
//
//    lastCounted = millis();
//  }
  
//  if (currentTime - lastUpdated >= 500) {
//    // update displays ever 10 milliseconds 
//    // updateDisplays should only be executed here
//    updateDisplays();
//    //Serial.print("Pin state: "); Serial.println(timerDisplay.pinStates[0]);
//    lastUpdated = millis();
//  }
}

void clearAllDisplays() {
  for (int i=0; i < numDisplays; i++) {
    displays[i]->clear();
  }
}

void updateDisplays() {
  Serial.println("Writing to displays");
  for (int i=0; i < numDisplays; i++) {
    int numPins = displays[i]->totalPins;
    Serial.print("  display: "); Serial.print(i), Serial.print(" num pins: "), Serial.println(numPins);
    for (int p=0; p < numPins; p++) {
      int pinNum = displays[i]->numPinsOffset + p;
      int pinVal = displays[i]->pinStates[0];
      if (pinVal > 0) {
        Serial.print(" pin "); Serial.print(pinNum); Serial.print(" val "); Serial.print(pinVal); Serial.print(", ");
      }
      tlc.setPWM(pinNum, pinVal);
    }
    Serial.println();
  }
  tlc.write();
  Serial.println("Done writing");
}
