#ifndef _POPSHOT_H
#define _POPSHOT_H

#include <Arduino.h>


class PopShot {
 public:
  PopShot(int lane);

  void setPWM(int chan, int pwm);
  int getPWM(int chan);
  void setAll(int pwm);

  int numPins;
  int laneNum;

 private:
  int *pwmbuffer;

};


#endif
