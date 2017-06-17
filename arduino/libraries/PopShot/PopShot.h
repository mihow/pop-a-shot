#ifndef _POPSHOT_H
#define _POPSHOT_H

#include <Arduino.h>


class PopShot {
 public:
  PopShot(uint16_t lane);

  void setPWM(uint16_t chan, uint16_t pwm);
  uint16_t getPWM(uint16_t chan);
  void setAll(uint16_t pwm);

  uint16_t numPins;
  uint16_t laneNum;

 private:
  uint16_t *pwmbuffer;

};


#endif
