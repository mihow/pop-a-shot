#include "SevenSeg.h"
#include "Adafruit_TLC5947.h"

// How many boards do you have chained?
#define NUM_TLC5974 2

#define data   46
#define clock   45
#define latch   47
#define numDisplays 2

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);

SevenSeg scoreDisplay(3, 0, 4095); // num digits, num pins offset, max brightness
SevenSeg timerDisplay(2, 24, 2048); // num digits, num pins offset, max brightness

SevenSeg *displays[numDisplays] = {&scoreDisplay, &timerDisplay};


int counter = 0;
int score = 0;

long startTime;
long elapsedTime;

void setup() {
  Serial.begin(9600);
  Serial.println("Setting up display...");

  clearAllDisplays();
  updateDisplays();
  
  startTime = millis();
  
  Serial.println("Ready!");
}

void loop() {
  elapsedTime = millis() - startTime;

  if (elapsedTime > 1000) {
    counter++;
    timerDisplay.set(counter);
  }
  
  if (elapsedTime > 10) {
    // update displays ever 10 milliseconds 
    updateDisplays();
  }
  
  startTime = millis();
}

void clearAllDisplays() {
  for (int i=0; i < numDisplays; i++) {
    displays[i]->clear();
  }
}

void updateDisplays() {
  for (int i=0; i < numDisplays; i++) {
    for (int p=0; i < displays[i]->getNumPins(); p++) {
      tlc.setPWM(
        displays[i]->numPinsOffset + p, 
        displays[i]->getPinState(p)
      );
    }
  }
  tlc.write();
}
