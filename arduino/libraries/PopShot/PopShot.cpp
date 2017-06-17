#include <PopShot.h>

PopShot::PopShot(int lane) {
  laneNum = lane;
  numPins = 4;

  //pwmbuffer = (int *)malloc(numPins);
  //memset(pwmbuffer, 0, numPins);
  pwmbuffer[numPins];
}

void PopShot::setAll(int pwm) {
  //for (int16_t c=24*numdrivers - 1; c >= 0 ; c--) {
  for(int c=0; c<numPins; c++) {
      pwmbuffer[c] = pwm;
  }
}

void PopShot::setPWM(int chan, int pwm) {
  pwmbuffer[chan] = pwm;  
}

int PopShot::getPWM(int chan) {
  return pwmbuffer[chan];
}
