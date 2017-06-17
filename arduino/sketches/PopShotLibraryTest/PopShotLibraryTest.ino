#include "PopShot.h"

PopShot machine = PopShot(1);

int counter = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  
  machine.setAll(0);
}

void loop() {
  machine.setPWM(3, 333);
  machine.setPWM(4, 444);
  machine.setPWM(0, counter);

  for(int i=0; i<=machine.numPins; i++) {
    int val = machine.getPWM(i);
    Serial.print("pin "); Serial.print(i); Serial.print(": "); Serial.println(val);
  }

  delay(1000);
  counter ++;

}
