#include <SevenSeg.h>
#include "Adafruit_TLC5947.h"

// How many boards do you have chained?
#define NUM_TLC5974 2

#define data   46
#define clock   45
#define latch   47

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);

SevenSeg scoreDisplay(6, 7, 2, 1, 0, 5, 4);
SevenSeg timerDisplay(6, 7, 2, 1, 0, 5, 4);



void setup() {
  Serial.begin(9600);
  Serial.println("Setting up display...");
  
  scoreDisplay.setNumDigits(3);
  //scoreDisplay.setOffset(0);
  //scoreDisplay.setBrightness(3000);
  
  scoreDisplay.setNumDigits(2);
  //timerDisplay.setOffset(3);
  //timerDisplay.setBrightess(4095);

  //tlc.clear();
  tlc.write();
  
  Serial.println("Ready!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
