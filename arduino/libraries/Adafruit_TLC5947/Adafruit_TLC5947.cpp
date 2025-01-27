/*************************************************** 
  This is a library for our Adafruit 24-channel PWM/LED driver

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


#include <Adafruit_TLC5947.h>

Adafruit_TLC5947::Adafruit_TLC5947(uint16_t n, uint8_t c, uint8_t d, uint8_t l, uint8_t o) {
  numdrivers = n;
  _clk = c;
  _dat = d;
  _lat = l;
  _oe  = o;

  //pwmbuffer = (uint16_t *)calloc(2, 24*n);
  pwmbuffer = (uint16_t *)malloc(2 * 24*n);
  memset(pwmbuffer, 0, 2*24*n);
}

//clock 45, data 46, latch 47

void Adafruit_TLC5947::writeFast(void) {
  PORTL &= ~_BV(PL2);  //latch to 47 is PL2
  // 24 channels per TLC5974
  for (int8_t c=24*numdrivers - 1; c >= 0 ; c--) {
    // 12 bits per channel, send MSB first
    for (int8_t b=11; b>=0; b--) {
      PORTL &= ~_BV(PL4); //clock to 45 is PL4

      if (pwmbuffer[c] & (1 << b))
      PORTL |= _BV(PL3); //,data to 46 is PL3
      else
      PORTL &= ~_BV(PL3);

      PORTL |= _BV(PL4);
    }
  }
   PORTL &= ~_BV(PL4);
   PORTL |= _BV(PL2);
   PORTL &= ~_BV(PL2);
}

void Adafruit_TLC5947::write(void) {
  digitalWrite(_lat, LOW);
  digitalWrite(_oe, LOW);
  // 24 channels per TLC5974
  for (int16_t c=24*numdrivers - 1; c >= 0 ; c--) {
    // 12 bits per channel, send MSB first
    for (int8_t b=11; b>=0; b--) {
      digitalWrite(_clk, LOW);
      
      if (pwmbuffer[c] & (1 << b))  
        digitalWrite(_dat, HIGH);
      else
        digitalWrite(_dat, LOW);

      digitalWrite(_clk, HIGH);
    }
  }
  digitalWrite(_clk, LOW);
  
  digitalWrite(_lat, HIGH);  
  digitalWrite(_oe, HIGH);

  digitalWrite(_lat, LOW);
  digitalWrite(_oe, LOW);
}



void Adafruit_TLC5947::setPWM(uint16_t chan, uint16_t pwm) {
  if (pwm > 4095) pwm = 4095;
  if (chan > 24*numdrivers) return;
  pwmbuffer[chan] = pwm;  
}


void Adafruit_TLC5947::setLED(uint16_t lednum, uint16_t r, uint16_t g, uint16_t b) {
  setPWM(lednum*3, r);
  setPWM(lednum*3+1, g);
  setPWM(lednum*3+2, b);
}


boolean Adafruit_TLC5947::begin() {
  if (!pwmbuffer) return false;

  pinMode(_clk, OUTPUT);
  pinMode(_dat, OUTPUT);
  pinMode(_lat, OUTPUT);
  pinMode(_oe, OUTPUT);
  digitalWrite(_lat, LOW);
  digitalWrite(_oe, LOW);

  return true;
}
