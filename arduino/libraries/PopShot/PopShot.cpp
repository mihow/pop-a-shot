#include <PopShot.h>

PopShot::PopShot(uint16_t lane) {
  laneNum = lane;
  numPins = 4;

  pwmbuffer = (uint16_t *)malloc(numPins);
  memset(pwmbuffer, 0, numPins);
}

void PopShot::setAll(uint16_t pwm) {
  //for (int16_t c=24*numdrivers - 1; c >= 0 ; c--) {
  for(uint16_t c=0; c<numPins; c++) {
      pwmbuffer[c] = pwm;
  }
}

void PopShot::setPWM(uint16_t chan, uint16_t pwm) {
  pwmbuffer[chan] = pwm;  
}

uint16_t PopShot::getPWM(uint16_t chan) {
  return pwmbuffer[chan];
}
