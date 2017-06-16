#include <SevenSegTLC.h>

segmentPinOrder = 6, 7, 2, 1, 0, 5, 4;

SevenSegTLC scoreDisplay(6, 7, 2, 1, 0, 5, 4);
SevenSegTLC timerDisplay(6, 7, 2, 1, 0, 5, 4);



void setup() {
  Serial.begin(9600);
  Serial.println("Setting up display...");
  
  scoreDisplay.setNumDigits(3)
  scoreDisplay.setOffset(0);
  scoreDisplay.setBrightness(3000);
  
  scoreDisplay.setNumDigits(2)
  timerDisplay.setOffset(3);
  timerDisplay.setBrightess(4095);

  tlc.clear()
  tlc.write()
  
  Serial.println("Ready!");
  
  startTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:

}
