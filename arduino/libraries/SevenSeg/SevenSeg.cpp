/*
  SevenSeg v.1.0
  SevenSeg.h - Library for controlling a 7-segment display
  Created by Sigvald Marholm, 02.06.2015.
  Modified to use the TLC5947 chip as a driver by Michael Bunsen, 01.09.2016
*/

#include "Arduino.h"
#include "SevenSeg.h"


// Constructor
SevenSeg::SevenSeg(int numDigits, int offset, int brightness){

  // Assume Common Anode (user must change this if false)
  setCommonAnode();

  // Set segment pins
  _A=6;
  _B=7;
  _C=2;
  _D=1;
  _E=0;
  _F=5;
  _G=4;
  _DP=-1;	// DP initially not assigned

  _numOfDigits = numDigits;
  _numOfSegments = 8; // Including dot
  _maxBrightness = brightness; // 4095
  _totalPins = _numOfSegments * _numOfDigits;

  numPinsOffset = offset; // 0

  _pinStates = (int *)malloc(2 * _totalPins);
  memset(_pinStates, 0, 2 * _totalPins);

  clear();
}

void SevenSeg::setPinPlacements(int A,int B,int C,int D,int E,int F,int G){
  _A=A;
  _B=B;
  _C=C;
  _D=D;
  _E=E;
  _F=F;
  _G=G;
  _DP=-1;	// DP initially not assigned
}

void SevenSeg::setCommonAnode(){
  _segOn=_maxBrightness;
  _segOff=0;
}

void SevenSeg::setCommonCathode(){
  _segOn=0;
  _segOff=_maxBrightness;
}

void SevenSeg::clear(){

  // Serial.println("");
  // Serial.println("Clearing Display");

  for(int i=0;i<_numOfDigits;i++){
    setSeg(i, _A, _segOff);
    setSeg(i, _B, _segOff);
    setSeg(i, _C, _segOff);
    setSeg(i, _D, _segOff);
    setSeg(i, _E, _segOff);
    setSeg(i, _F, _segOff);
    setSeg(i, _G, _segOff);
  }

}

void SevenSeg::set(int num){

    // Serial.println("");
    // Serial.print("Showing number: ");
    // Serial.println(num);

    for(int i=_numOfDigits-1;i>=0;i--){
        int nextPiece = num % 10L;
        if(num || i==_numOfDigits-1){
            setDigit(i, nextPiece);
        } else {
            // Fill digits with zero
            setDigit(i, 0);
        }
        num /= 10;
    }
}

void SevenSeg::setDigit(int digit, int num){

  if (digit > _numOfDigits) {
      
      // We don't have enough digits. Cut this number off
      
  } else {
  
      //Serial.print("    Setting digit '");
      //Serial.print(digit);
      //Serial.print("' to: ");
      //Serial.println(num);

      // Turn off all LEDs
      setSeg(digit, _A, _segOff);
      setSeg(digit, _B, _segOff);
      setSeg(digit, _C, _segOff);
      setSeg(digit, _D, _segOff);
      setSeg(digit, _E, _segOff);
      setSeg(digit, _F, _segOff);
      setSeg(digit, _G, _segOff);
      
      if(num==1){
        setSeg(digit, _B, _segOn);
        setSeg(digit, _C, _segOn);
      }                     
                            
      if(num==2){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _G, _segOn);
        setSeg(digit, _E, _segOn);
        setSeg(digit, _D, _segOn);
      }                     
                            
      if(num==3){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _G, _segOn);
        setSeg(digit, _C, _segOn);
        setSeg(digit, _D, _segOn);
      }                     
                            
      if(num==4){       
        setSeg(digit, _F, _segOn);
        setSeg(digit, _G, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _C, _segOn);
      }                     
                            
      if(num==5){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _F, _segOn);
        setSeg(digit, _G, _segOn);
        setSeg(digit, _C, _segOn);
        setSeg(digit, _D, _segOn);
      }                     
                            
      if(num==6){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _F, _segOn);
        setSeg(digit, _E, _segOn);
        setSeg(digit, _D, _segOn);
        setSeg(digit, _C, _segOn);
        setSeg(digit, _G, _segOn);
      }                     
                            
      if(num==7){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _C, _segOn);
      }                     
                            
      if(num==8){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _C, _segOn);
        setSeg(digit, _D, _segOn);
        setSeg(digit, _E, _segOn);
        setSeg(digit, _F, _segOn);
        setSeg(digit, _G, _segOn);
      }                     
                            
      if(num==9){       
        setSeg(digit, _G, _segOn);
        setSeg(digit, _F, _segOn);
        setSeg(digit, _A, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _C, _segOn);
        setSeg(digit, _D, _segOn);
      }                     
                            
      if(num==0){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _C, _segOn);
        setSeg(digit, _D, _segOn);
        setSeg(digit, _E, _segOn);
        setSeg(digit, _F, _segOn);
      }

  }
      
}

void SevenSeg::setSeg(int digit, int seg, int pwm_val) {
      //Serial.print("        Setting pin '");
      //Serial.print(pinNum(digit, seg));
      //Serial.print("' to: ");
      //Serial.println(pwm_val);

      //tlc.setPWM(pinNum(digit, seg), pwm_val);
      _pinStates[pinNum(digit, seg), pwm_val];
}

int SevenSeg::getNumPins() {
    return _totalPins;
}

int SevenSeg::pinNum(int digit, int seg) {
    return seg + (digit * _numOfSegments);
}

int SevenSeg::getPinState(int pin) {
    return _pinStates[pin];
}

int * SevenSeg::getPinStates() {
    return _pinStates;
}
