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

    // Low level functions for initializing hardware
    void setCommonAnode();
    void setCommonCathode();
    void setActivePinState(int,int);
    void setPinPlacements(int,int,int,int,int,int,int);
    int numPinsOffset;

    // Low level functions for printing to display
    void clear();
    void changeDigit(int);
    void changeDigit(char);
    void setDigit(int, int);
    void setDigit(char);
    void setSeg(int, int, int);
    void setDP();
    void clearDP();
    int  pinNum(int, int);
    int  getNumPins();
    int  getPinState(int);
    int  *getPinStates();

    // High level functions for printing to display
    void set(long int);
    void set(int);
    void set(long int,int);
    void set(int, int);
    void set(char*);
    void set(String);
    void set(double);
    void set(double num, int point);
    void setClock(int,int,char);
    void setClock(int,int);
    void setClock(int,char);
    void setClock(int);

    // Timer control functions
    void setTimer(int);
    void clearTimer();
    void interruptAction();
    void startTimer();
    void stopTimer();

    // To clean up
//  void setPinState(int);	// I think this isn't in use. Its called setActivePinState?
//  int getDigitDelay();	// How many get-functions should I make?

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

    // PWM value for "seg on"
    int _maxBrightness;

    // The pins for each of the digits
    int *_dig;
    int _numOfDigits;
    int _numOfSegments;

    // The pins values for all of the digits in the display
    int _totalPins;
    int *_pinStates;

    // Timing variables. Stored in microseconds.
    long int _digitDelay;		// How much time spent per display during multiplexing.
    long int _digitOnDelay;		// How much on-time per display (used for dimming), i.e. it could be on only 40% of digitDelay
    long int _digitOffDelay;		// digitDelay minus digitOnDelay
    int _dutyCycle;		// The duty cycle (digitOnDelay/digitDelay, here in percent)
    // Strictly speaking, _digitOnDelay and _digitOffDelay holds redundant information, but are stored so the computations only
    // needs to be made once. There's an internal update function to update them based on the _digitDelay and _dutyCycle

    void updDelay();
    void execDelay(int);	// Executes delay in microseconds
    char iaExtractDigit(long int,int,int);
    long int iaLimitInt(long int);

    // Sets which values (HIGH or LOW) pins should have to turn on/off segments or digits.
    // This depends on whether the display is Common Anode or Common Cathode.
    int _digOn;
    int _digOff;
    int _segOn;
    int _segOff;

    // Variables used by interrupt service routine to keep track of stuff
    int _timerDigit;		// What digit interrupt timer should update next time
    int _timerPhase;		// What phase of the cycle it is to update, i.e. phase 1 (on), or phase 0 (off). Needed for duty cycling.
    int _timerID;		// Values 0,1,2 corresponds to using timer0, timer1 or timer2.
    long int _timerCounter;		// Prescaler of 64 is used since this is available on all timers (0, 1 and 2).
				// Timer registers are not sufficiently large. This counter variable will extend upon the original timer.
				// and increment by one each time.
    long int _timerCounterOnEnd;	// How far _timerCounter should count to provide a delay approximately equal to _digitOnDelay
    long int _timerCounterOffEnd;	// How far _timerCounter should count to provide a delay approximately equal to _digitOffDelay

    // What is to be printed by interruptAction is determined by these variables
    long int _writeInt;		// Holds the number to be written in case of int, fixed point, or clock
    int _writePoint;		// Holds the number of digits to use as decimals in case of fixed point
//    float _writeFloat;		// Holds the float to write in case of float. OBSOLETE: Float are converted to fixed point
    char *_writeStr;		// Holds a pointer to a string to write in case of string
    char _writeMode;		// 'p' for fixed point, 'i' for integer, 'f' for float, ':'/'.'/'_' for clock with according divisor symbol
    String _writeStrObj;

};

#endif
