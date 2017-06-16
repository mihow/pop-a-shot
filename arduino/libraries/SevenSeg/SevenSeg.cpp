/*
  SevenSeg v.1.0
  SevenSeg.h - Library for controlling a 7-segment display
  Created by Sigvald Marholm, 02.06.2015.
  Modified to use the TLC5947 chip as a driver by Michael Bunsen, 01.09.2016
*/

#include "Arduino.h"
#include "SevenSeg.h"

//int maxBrightness = 4095;
int maxBrightness = 4095;


// Constructor
SevenSeg::SevenSeg(int A,int B,int C,int D,int E,int F,int G){

  // Assume Common Anode (user must change this if false)
  setCommonAnode();

  // Set segment pins
  _A=A;
  _B=B;
  _C=C;
  _D=D;
  _E=E;
  _F=F;
  _G=G;
  _DP=-1;	// DP initially not assigned

  // Assume no digit pins are used (i.e. it's only one hardwired digit)
  _numOfDigits=0;
  _numOfSegments=8; // Including dot

  // Clear display
  clearDisp();
}

void SevenSeg::setCommonAnode(){
  _segOn=maxBrightness;
  _segOff=0;
}

void SevenSeg::setCommonCathode(){
  _segOn=0;
  _segOff=maxBrightness;
}

void SevenSeg::clearDisp(){

  // Serial.println("");
  // Serial.println("Clearing Display");

  for(int i=0;i<_numOfDigits;i++){
    writeSeg(i, _A, _segOff);
    writeSeg(i, _B, _segOff);
    writeSeg(i, _C, _segOff);
    writeSeg(i, _D, _segOff);
    writeSeg(i, _E, _segOff);
    writeSeg(i, _F, _segOff);
    writeSeg(i, _G, _segOff);
  }

  applyChanges();

}

void SevenSeg::setNumDigits(int numOfDigits){
  _numOfDigits=numOfDigits;

  // Serial.print("New display with this many digits: ");
  // Serial.println(_numOfDigits);
}

void SevenSeg::write(int num){

    // Serial.println("");
    // Serial.print("Showing number: ");
    // Serial.println(num);

    for(int i=_numOfDigits-1;i>=0;i--){
        int nextPiece = num % 10L;
        if(num || i==_numOfDigits-1){
            writeDigit(i, nextPiece);
        } else {
            // Fill digits with zero
            writeDigit(i, 0);
        }
        num /= 10;
    }
}

void SevenSeg::writeDigit(int digit, int num){

  if (digit > _numOfDigits) {
      
      // We don't have enough digits. Cut this number off
      
  } else {
  
      //Serial.print("    Setting digit '");
      //Serial.print(digit);
      //Serial.print("' to: ");
      //Serial.println(num);

      // Turn off all LEDs
      writeSeg(digit, _A, _segOff);
      writeSeg(digit, _B, _segOff);
      writeSeg(digit, _C, _segOff);
      writeSeg(digit, _D, _segOff);
      writeSeg(digit, _E, _segOff);
      writeSeg(digit, _F, _segOff);
      writeSeg(digit, _G, _segOff);
      
      if(num==1){
        writeSeg(digit, _B, _segOn);
        writeSeg(digit, _C, _segOn);
      }                     
                            
      if(num==2){       
        writeSeg(digit, _A, _segOn);
        writeSeg(digit, _B, _segOn);
        writeSeg(digit, _G, _segOn);
        writeSeg(digit, _E, _segOn);
        writeSeg(digit, _D, _segOn);
      }                     
                            
      if(num==3){       
        writeSeg(digit, _A, _segOn);
        writeSeg(digit, _B, _segOn);
        writeSeg(digit, _G, _segOn);
        writeSeg(digit, _C, _segOn);
        writeSeg(digit, _D, _segOn);
      }                     
                            
      if(num==4){       
        writeSeg(digit, _F, _segOn);
        writeSeg(digit, _G, _segOn);
        writeSeg(digit, _B, _segOn);
        writeSeg(digit, _C, _segOn);
      }                     
                            
      if(num==5){       
        writeSeg(digit, _A, _segOn);
        writeSeg(digit, _F, _segOn);
        writeSeg(digit, _G, _segOn);
        writeSeg(digit, _C, _segOn);
        writeSeg(digit, _D, _segOn);
      }                     
                            
      if(num==6){       
        writeSeg(digit, _A, _segOn);
        writeSeg(digit, _F, _segOn);
        writeSeg(digit, _E, _segOn);
        writeSeg(digit, _D, _segOn);
        writeSeg(digit, _C, _segOn);
        writeSeg(digit, _G, _segOn);
      }                     
                            
      if(num==7){       
        writeSeg(digit, _A, _segOn);
        writeSeg(digit, _B, _segOn);
        writeSeg(digit, _C, _segOn);
      }                     
                            
      if(num==8){       
        writeSeg(digit, _A, _segOn);
        writeSeg(digit, _B, _segOn);
        writeSeg(digit, _C, _segOn);
        writeSeg(digit, _D, _segOn);
        writeSeg(digit, _E, _segOn);
        writeSeg(digit, _F, _segOn);
        writeSeg(digit, _G, _segOn);
      }                     
                            
      if(num==9){       
        writeSeg(digit, _G, _segOn);
        writeSeg(digit, _F, _segOn);
        writeSeg(digit, _A, _segOn);
        writeSeg(digit, _B, _segOn);
        writeSeg(digit, _C, _segOn);
        writeSeg(digit, _D, _segOn);
      }                     
                            
      if(num==0){       
        writeSeg(digit, _A, _segOn);
        writeSeg(digit, _B, _segOn);
        writeSeg(digit, _C, _segOn);
        writeSeg(digit, _D, _segOn);
        writeSeg(digit, _E, _segOn);
        writeSeg(digit, _F, _segOn);
      }

      applyChanges();
  }
      
}

void SevenSeg::writeSeg(int digit, int seg, int pwm_val) {
      //Serial.print("        Setting pin '");
      //Serial.print(pinNum(digit, seg));
      //Serial.print("' to: ");
      //Serial.println(pwm_val);

      //tlc.setPWM(pinNum(digit, seg), pwm_val);
}

void SevenSeg::applyChanges() {
    //Serial.println("Writing all changes");
    //tlc.write();
}

int SevenSeg::pinNum(int digit, int seg) {
    return seg + (digit * _numOfSegments);
}

void SevenSeg::cyclePins() {
    clearDisp();

    for(int i=0; i<8*_numOfDigits; i++) {
        //tlc.setPWM(i, _segOn);
        //tlc.write();
    }

    for(int i=8*_numOfDigits; i<0; i--) {
        //tlc.setPWM(i, _segOff);
        //tlc.write();
    }

    clearDisp();
}
