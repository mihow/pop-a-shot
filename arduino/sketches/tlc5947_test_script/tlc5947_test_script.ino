/*************************************************** 
  This is an example for our Adafruit 24-channel PWM/LED driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1429

  These drivers uses SPI to communicate, 3 pins are required to  
  interface: Data, Clock and Latch. The boards are chainable

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_TLC5947.h"

// How many boards do you have chained?
#define NUM_TLC5974 2
#define NUM_DIGITS 5

//display 1 is 43,42,44 data,clock,latch
//display 2 is 46,45,47 data,clock,latch

#define data   46
#define clock   45
#define latch   47

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);

void setup() {
  Serial.begin(9600);
  
  Serial.println("TLC5974 test");
  tlc.begin();
}

void loop() {
  cycle(100, 4095);
  delay(500);
  
  setAll(0);
  delay(500);
  
  //fadeIn(100);
  delay(500);
  
  setAll(0);
  delay(500);
  
  blink(500);
  blink(500);
  delay(500);

  setAll(0);
  delay(500);
}

void blink(int wait) {
  setAll(4095);
  delay(wait);
  setAll(0);
  delay(wait);
}

void fadeIn(int interval) {
  for(int i=0; i<4095; i=i+interval) {
    setAll(i);
  }
}

void setAll(int brightness) {
  for(uint16_t i=0; i<8*NUM_DIGITS; i++) {
      tlc.setPWM(i, brightness);
  }
  tlc.write();
}

void cycle(int wait, int brightness) {
  for(uint16_t i=0; i<8*NUM_DIGITS; i++) {
      tlc.setPWM(i, brightness);
      tlc.write();
      delay(wait);
  }
  for(uint16_t i=8*NUM_DIGITS; i<0; i--) {
      tlc.setPWM(i, 0);
      tlc.write();
      delay(wait);
  }
}

