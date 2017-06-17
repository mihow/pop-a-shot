/*
  SevenSeg v.1.0
  SevenSeg.h - Library for controlling a 7-segment display
  Created by Sigvald Marholm, 02.06.2015.
  Modified to use the TLC5947 chip as a driver by Michael Bunsen, 01.09.2016
*/

#include "Arduino.h"
#include "SevenSeg.h"


// Constructor
SevenSeg::SevenSeg(int digits, int offset, int brightness){

  // Set segment pins
  _A=6;
  _B=7;
  _C=2;
  _D=1;
  _E=0;
  _F=5;
  _G=4;
  _DP=-1;	// DP initially not assigned

  numDigits = digits;
  numSegments = 8; // Including dot
  totalPins = numSegments * numDigits;

  numPinsOffset = offset; // 0

  // Assume Common Anode (user must change this if false)
  _segOn=brightness;
  _segOff=0;

  pinStates = (uint16_t *)malloc(totalPins);
  memset(pinStates, 0, totalPins);

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

void SevenSeg::clear(){

  // Serial.println("");
  // Serial.println("Clearing Display");

  for(int i=0;i<numDigits;i++){
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

    Serial.println("");
    Serial.print("Showing number: ");
    Serial.println(num);

    for(int i=numDigits-1;i>=0;i--){
        int nextPiece = num % 10L;
        if(num || i==numDigits-1){
            setDigit(i, nextPiece);
        } else {
            // Fill digits with zero
            setDigit(i, 0);
        }
        num /= 10;
    }
}

void SevenSeg::setDigit(int digit, int num){

  if (digit > numDigits) {
      
      // We don't have enough digits. Cut this number off
      
  } else {
  
      Serial.print("    Setting digit '");
      Serial.print(digit);
      Serial.print("' to: ");
      Serial.println(num);

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
                            
      else if(num==2){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _G, _segOn);
        setSeg(digit, _E, _segOn);
        setSeg(digit, _D, _segOn);
      }                     
                            
      else if(num==3){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _G, _segOn);
        setSeg(digit, _C, _segOn);
        setSeg(digit, _D, _segOn);
      }                     
                            
      else if(num==4){       
        setSeg(digit, _F, _segOn);
        setSeg(digit, _G, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _C, _segOn);
      }                     
                            
      else if(num==5){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _F, _segOn);
        setSeg(digit, _G, _segOn);
        setSeg(digit, _C, _segOn);
        setSeg(digit, _D, _segOn);
      }                     
                            
      else if(num==6){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _F, _segOn);
        setSeg(digit, _E, _segOn);
        setSeg(digit, _D, _segOn);
        setSeg(digit, _C, _segOn);
        setSeg(digit, _G, _segOn);
      }                     
                            
      else if(num==7){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _C, _segOn);
      }                     
                            
      else if(num==8){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _C, _segOn);
        setSeg(digit, _D, _segOn);
        setSeg(digit, _E, _segOn);
        setSeg(digit, _F, _segOn);
        setSeg(digit, _G, _segOn);
      }                     
                            
      else if(num==9){       
        setSeg(digit, _G, _segOn);
        setSeg(digit, _F, _segOn);
        setSeg(digit, _A, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _C, _segOn);
        setSeg(digit, _D, _segOn);
      }                     
                            
      else if(num==0){       
        setSeg(digit, _A, _segOn);
        setSeg(digit, _B, _segOn);
        setSeg(digit, _C, _segOn);
        setSeg(digit, _D, _segOn);
        setSeg(digit, _E, _segOn);
        setSeg(digit, _F, _segOn);
      }

      else {
        Serial.print("Unknown number or bad type to print: ");
        Serial.println(num);
      }

  }

  printPinStates();
      
}

void SevenSeg::setSeg(int digit, int seg, int pwm_val) {
      //Serial.print("        Setting pin '");
      //Serial.print(pinNum(digit, seg));
      //Serial.print("' to: ");
      //Serial.println(pwm_val);

      //tlc.setPWM(pinNum(digit, seg), pwm_val);
      int pin = pinNum(digit, seg);
      pinStates[pin] = pwm_val;
}

int SevenSeg::pinNum(int digit, int seg) {
    return seg + (digit * numSegments);
}

//int * SevenSeg::getPinStates() {
//    return pinStates;
//}
//

void SevenSeg::printPinStates() {
  for(int i=0; i<totalPins; i++) {
      Serial.print("Pin "); Serial.print(i); Serial.print(": "); Serial.println(pinStates[0]);
  }
}
