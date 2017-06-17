/*
  SevenSeg v1.1
  SevenSeg.h - Library for controlling a 7-segment LCD
  Created by Sigvald Marholm, 02.06.2015.
  Modified to use the 5947 chip as a driver by Michael Bunsen, 01.09.2016
*/

#ifndef SevenSeg_h
#define SevenSeg_h

#include "Arduino.h"

class SevenSeg
{

  public:

    // Constructor
    SevenSeg(int,int,int);

    void setPinPlacements(int,int,int,int,int,int,int);
    int numPinsOffset;

    // PWM value for "seg on"
    int maxBrightness;

    // The pins for each of the digits
    int numDigits;
    int numSegments;

    // Low level functions for printing to display
    void clear();
    void setDigit(int, int);
    void setSeg(int, int, int);

    // The pins values for all of the digits in the display
    uint16_t  *pinStates;
    void printPinStates(); 
    int  totalPins;
    int  pinNum(int, int);
    //int  *getPinStates();

    // High level functions for printing to display
    void set(int);

  private:

    // The pins for each of the seven segments (eight with decimal point)
    int _A;
    int _B;
    int _C;
    int _D;
    int _E;
    int _F;
    int _G;
    int _DP;	// -1 when decimal point not assigned

    int _segOn;
    int _segOff;

};

#endif
