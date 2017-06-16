/*
* Test for the TLC5947 without any external libraries
* Taken from "Voice controlled LED heart" by Aditya K.
* License:	MIT
*/

//#define MAX_PWM 4095
#define MAX_PWM 2000
#define MIN_PWM 0

/* PINS FOR REGULAR ARDUINO */
// #define DATA  PD4
// #define CLOCK PD5
// #define LATCH PD6

/* PINS FOR POP-A-SHOT SHIELD + MEGA */
#define CLOCK   45
#define DATA   46
#define LATCH   47

//  Pull OE pin HIGH to disable TLC
/* N/A FOR POP-A-SHOT SHIELD */
//#define OE    PD7 // 8

// Toggle pin HIGH to turn off TLC
//#define TLC_DISABLE() (PORTB |= bit(OE))

// Toggle pin LOW to turn on TLC
//#define TLC_ENABLE()  (PORTB &= ~bit(OE))

#define DATA_HIGH()   (PORTD |= bit(DATA))
#define DATA_LOW()    (PORTD &= ~bit(DATA))

#define CLOCK_HIGH()  ((PORTD |= bit(CLOCK)))
#define CLOCK_LOW()   ((PORTD &= ~bit(CLOCK)))


#define LATCH_ENABLE()  ((PORTD |= bit(LATCH)))
#define LATCH_DISABLE() ((PORTD &= ~bit(LATCH)))


uint16_t pwm_buffer[24];
uint8_t receive[10];

static bool reply = false;
unsigned long start;
const uint16_t wait  = 8000;



//TLC5947 related functions
void init_tlc() {
  memset(pwm_buffer, 0, 24);
  pinMode(DATA, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  //pinMode(OE, OUTPUT);
  LATCH_DISABLE();
  //TLC_DISABLE();
}

inline void tlc_set_pwm(uint16_t chan, uint16_t pwm) {
  pwm_buffer[chan] = pwm;
}

inline void tlc_clear_buff(void) {
  for (uint8_t i = 0; i < 24; i++)
    pwm_buffer[i] = 0;
}


void tlc_write(void) {
  LATCH_DISABLE();

  for (int t = 23; t >= 0; t--) {
    for (int b = 11; b >= 0; b--) {
      CLOCK_LOW();
      if (pwm_buffer[t] & (1 << b)) {
        DATA_HIGH();
      } else {
        DATA_LOW();
      }
      CLOCK_HIGH();
    }
  }
  CLOCK_LOW();
  LATCH_ENABLE();
  LATCH_DISABLE();
}

// code reference: https://docs.micropython.org/en/latest/pyboard/pyboard/tutorial/fading_led.html
void heartbeat_effect() {

  uint16_t curr_steps = 0;
  uint16_t step_incr = 117; // total of 35 steps: 117*35  = 4095

  while (1) {
    for (uint8_t i = 0; i < 24; i++) {
      tlc_set_pwm(i, curr_steps);
    }
    tlc_write();
    delay(45);
    curr_steps += step_incr;

    if (curr_steps > MAX_PWM) {
      curr_steps = MAX_PWM;
      step_incr *= -1;
    }
    else if (curr_steps < MIN_PWM) {
      curr_steps = MIN_PWM;
      step_incr *= -1;
    }
  }
}


void heart_effect() {
  uint8_t left_led;
  uint8_t right_led;
  for (left_led = 0, right_led = 23; left_led <= 12 && right_led >= 12; left_led++, right_led--) {
    tlc_set_pwm(left_led, MAX_PWM);
    tlc_set_pwm(right_led, MAX_PWM);
    tlc_write();
    delay(100);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing TLC Driver");
  init_tlc();
  Serial.println("Clearing buffer");
  tlc_clear_buff();
  Serial.println("Writing");
  tlc_write();
  //Serial.println("Setting enable");
  //TLC_ENABLE();
}

void loop() {
  tlc_set_pwm(10, 2000);
}
