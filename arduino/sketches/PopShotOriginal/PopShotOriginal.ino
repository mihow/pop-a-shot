#include "Adafruit_TLC5947.h"

// How many boards do you have chained?
#define NUM_TLC5974 6

#define clock   45
#define data    46
#define latch   47

//// Test config for plain Arduino:
//#define clock   4
//#define data    5
//#define latch   6

Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, clock, data, latch);

int number = 0;
int timeTens, timeOnes, pointsHundies, pointsTens, pointsOnes;
int time = 0;
int startTime = 45;
int points = 0;
int currTimeMillis, timeMillis, prevTimeMillis, prevTesterClockMillis;
int prevDebugMillis;
int sensorOnMillis, shotStartMillis;
int scoreMillis, prevScoreMillis;
int startButton, prevStartButton, multiButton, prevMultiButton, sensor, prevSensor;
int testerMode, prevTesterMode;
int timeBetweenShots = 300;
int testerClock = 0;

int mode = 0; //0 = waiting, 1= start button pressed, 2 = playing, 3 = game over, 4 = you have pressed MULTI, 5 = someone else has pressed MULTI,
//6 = you and someone else have pressed multi, 7 = you have released multi but someone else is still holding it
//8 = you are the last one holding multi 9 = tester mode

int ON = 4095;
int OFF = 0;

int numbers[10][7] = {
  {ON, ON, ON, ON, ON, ON, OFF}, //zero
  {OFF, ON, ON, OFF, OFF, OFF, OFF}, //one
  {ON, ON, OFF, ON, ON, OFF, ON}, //two
  {ON, ON, ON, ON, OFF, OFF, ON}, //three
  {OFF, ON, ON, OFF, OFF, ON, ON}, //four
  {ON, OFF, ON, ON, OFF, ON, ON}, //five
  {ON, OFF, ON, ON, ON, ON, ON}, //six
  {ON, ON, ON, OFF, OFF, OFF, OFF}, //seven
  {ON, ON, ON, ON, ON, ON, ON}, //eight
  {ON, ON, ON, OFF, OFF, ON, ON} //niner
};


//int numbers[10][7] = {{1,1,1,1,1,1,0},//zero
//                      {0,1,1,0,0,0,0},//one
//                      {1,1,0,1,1,0,1},//two
//                      {1,1,1,1,0,0,1},//three
//                      {0,1,1,0,0,1,1},//four
//                      {1,0,1,1,0,1,1},//five
//                      {1,0,1,1,1,1,1},//six
//                      {1,1,1,0,0,0,0},//seven
//                      {1,1,1,1,1,1,1},//eight
//                      {1,1,1,0,0,1,1}};//niner


// Hoop 4
// int displaySet = 0; // Hoop 4 = 0, Hoop 3 = 1, Hoop 2 = 2, Hoop 1 = 3;
// int startButtonPin = 41;//NOTE THIS IS LOW=TRUE
// int startButtonLight = 8;
// int multiButtonPin = 40;//NOTE THIS IS LOW=TRUE
// int multiButtonLight = 7;
// int sensorPin = 58; //HOOOOOOOOOOP

// Hoop 3
int displaySet = 1;
int startButtonPin = 38;
int startButtonLight = 6;
int multiButtonPin = 37;
int multiButtonLight = 9;
int sensorPin = 59;

// Hoop 2
// int displaySet = 2;
// int startButtonPin = 9999;
// int startButtonLight = 9999;
// int multiButtonPin = 9999;
// int multiButtonLight = 9999;
// int sensorPin = 57;

//// Test config for plain Arduino:
//int startButtonPin = 8;
//int startButtonLight = 13;
//int multiButtonPin = -1;
//int multiButtonLight = -1;
//int sensorPin = 9;


// Seven Segment Display Pins
int segA = 6;
int segB = 7;
int segC = 2;
int segD = 1;
int segE = 0;
int segF = 5;
int segG = 4;
int segDot = -1; // Dot not used

int pinsPerDisplaySet = 8*6;
int offset = displaySet * pinsPerDisplaySet;

// Digit 5 // Timer digit 2 // Tens
int t1a = segA + (8 * 4) + offset; int t1b = segB + (8 * 4) + offset; int t1c = segC + (8 * 4) + offset; int t1d = segD + (8 * 4) + offset; int t1e =  segE + (8 * 4) + offset; int t1f = segF + (8 * 4) + offset; int t1g = segG + (8 * 4) + offset; //time tens

// Digit 4 // Timer digit 1 // Ones
int t2a = segA + (8 * 3) + offset; int t2b = segB + (8 * 3) + offset; int t2c = segC + (8 * 3) + offset; int t2d = segD + (8 * 3) + offset; int t2e = segE + (8 * 3) + offset; int t2f = segF + (8 * 3) + offset; int t2g = segG + (8 * 3) + offset; //time ones

// Digit 3 // Points digit 3 // Hundies
int p1a = segA + (8 * 2) + offset; int p1b = segB + (8 * 2) + offset; int p1c = segC + (8 * 2) + offset; int p1d = segD + (8 * 2) + offset; int p1e = segE + (8 * 2) + offset; int p1f = segF + (8 * 2) + offset; int p1g = segG + (8 * 2) + offset; //points hundies

// Digit 2 // Points digit 2 // Tens
int p2a = segA + (8 * 1) + offset; int p2b = segB + (8 * 1) + offset; int p2c = segC + (8 * 1) + offset; int p2d = segD + (8 * 1) + offset; int p2e = segE + (8 * 1) + offset; int p2f = segF + (8 * 1) + offset; int p2g = segG + (8 * 1) + offset; //points tens

// Digit 1 // Points digit 1 // Ones
int p3a = segA + (8 * 0) + offset; int p3b = segB + (8 * 0) + offset; int p3c = segC + (8 * 0) + offset; int p3d = segD + (8 * 0) + offset; int p3e = segE + (8 * 0) + offset; int p3f = segF + (8 * 0) + offset; int p3g = segG + (8 * 0) + offset; //points ones




void setup() {
  Serial.begin(9600);
  Serial.println("Setting Up");
  tlc.begin();

  pinMode(startButtonPin, INPUT_PULLUP); pinMode(multiButtonPin, INPUT_PULLUP); pinMode(sensorPin, INPUT);
  pinMode(startButtonLight, OUTPUT); pinMode(multiButtonLight, OUTPUT);

  digitalWrite(startButtonLight, HIGH);
  digitalWrite(multiButtonLight, HIGH);

  //pinMode(t1a, OUTPUT);pinMode(t1b, OUTPUT);pinMode(t1c, OUTPUT);pinMode(t1d, OUTPUT);pinMode(t1e, OUTPUT);pinMode(t1f, OUTPUT);pinMode(t1g, OUTPUT);
  //pinMode(t2a, OUTPUT);pinMode(t2b, OUTPUT);pinMode(t2c, OUTPUT);pinMode(t2d, OUTPUT);pinMode(t2e, OUTPUT);pinMode(t2f, OUTPUT);pinMode(t2g, OUTPUT);
  //pinMode(p1a, OUTPUT);pinMode(p1b, OUTPUT);pinMode(p1c, OUTPUT);pinMode(p1d, OUTPUT);pinMode(p1e, OUTPUT);pinMode(p1f, OUTPUT);pinMode(p1g, OUTPUT);
  //pinMode(p2a, OUTPUT);pinMode(p2b, OUTPUT);pinMode(p2c, OUTPUT);pinMode(p2d, OUTPUT);pinMode(p2e, OUTPUT);pinMode(p2f, OUTPUT);pinMode(p2g, OUTPUT);
  //pinMode(p3a, OUTPUT);pinMode(p3b, OUTPUT);pinMode(p3c, OUTPUT);pinMode(p3d, OUTPUT);pinMode(p3e, OUTPUT);pinMode(p3f, OUTPUT);pinMode(p3g, OUTPUT);

}

void loop() {
  currTimeMillis = millis();
  startButton = digitalRead(startButtonPin);
  multiButton = digitalRead(multiButtonPin);
  sensor = digitalRead(sensorPin);

  digitalWrite(startButtonLight, startButton);
  digitalWrite(multiButtonLight, multiButton);

  ///////WAITING
  if (mode == 0) {
    if (startButton == 0) {
      prevTimeMillis = currTimeMillis;
      mode = 1;
    }
  }

  //////START BUTTON PRESSED
  if (mode == 1) {
    if (startButton == 1) {
      if ((currTimeMillis - prevTimeMillis) > 100) { ///have this on every change of state that involves a button in order to debounce
        mode = 2;
        points = 0;
        prevScoreMillis = currTimeMillis;
        prevTimeMillis = currTimeMillis;
        prevTesterClockMillis = currTimeMillis;
        time = startTime;
      }
    }
  }

  ///////////////PLAYING
  if (mode == 2) {
    if (sensor == LOW && prevSensor == HIGH) {
      shotStartMillis = currTimeMillis;
      prevSensor = LOW;
    }
    if (sensor == HIGH && prevSensor == LOW) {
      sensorOnMillis = currTimeMillis - shotStartMillis;
      Serial.print("+ Sensor on time: "); Serial.println(sensorOnMillis);
      if (sensorOnMillis > 20 && sensorOnMillis < 300) { // Look like a ball passing or noise?
        if (currTimeMillis - prevScoreMillis > 400) { // How fast can consecutive shots be realistically?
          Serial.println("+ Score!");
          Serial.print("+ Time since last shot: "); Serial.println(currTimeMillis - prevScoreMillis);
          if (time > 10) {
            // Two pointer
            points += 2;
          } else {
            // Three pointers for last 10 seconds
            points += 3;
          }
          prevScoreMillis = currTimeMillis;
        } else {
          // Serial.print("- Ignoring shot, too soon since last one");
        }
      } else {
        // Serial.println("- Ignoring shot, didn't look like a ball (probably net or noise)");
      }
      prevSensor = HIGH;
    }

  //  ///////////////PLAYING
  //  if (mode == 2) {
  //    if (sensor != prevSensor) {
  //      if (sensor == 1) {
  //        scoreMillis = currTimeMillis;
  //        if ((scoreMillis - prevScoreMillis) > 500) {
  //          prevScoreMillis = scoreMillis;
  //          if (time > 10) {
  //            points += 2;
  //          } else {
  //            points += 3;
  //          }
  //        }
  //      }
  //    }


  if ((currTimeMillis - prevTimeMillis) > 1000) {
    prevTimeMillis = currTimeMillis;
    if (time > 1) {
      time -= 1;
    } else {
      time = 0;
      mode = 3;
    }
  }
}


///////////////GAME OVER
if (mode == 3) {
  if ((currTimeMillis - prevTimeMillis) > 1000) {
    prevTimeMillis = currTimeMillis;
    mode = 0;
  }
}



if (mode != 9) {
  timeOnes = time % 10;
  timeTens = (time - timeOnes) / 10;

  pointsOnes = points % 10;
  pointsTens = ((points - pointsOnes) / 10) % 10;
  pointsHundies = ((points - (pointsTens + pointsOnes)) / 100) % 10;
}
/*
  //the following was very useful for debugging, just comment out the above lines that define points and
  //the points tlcs can be used to show mode, multiBUtton etc..
  pointsOnes = mode%10;
  pointsTens = multiButton;
  pointsHundies = someoneElsePressedMulti;
*/

writeDisplay();

//  digitalWrite(t1a, numbers[timeTens][0]);  digitalWrite(t1b, numbers[timeTens][1]);  digitalWrite(t1c, numbers[timeTens][2]);  digitalWrite(t1d, numbers[timeTens][3]);  digitalWrite(t1e, numbers[timeTens][4]);  digitalWrite(t1f, numbers[timeTens][5]);  digitalWrite(t1g, numbers[timeTens][6]);
//  digitalWrite(t2a, numbers[timeOnes][0]);  digitalWrite(t2b, numbers[timeOnes][1]);  digitalWrite(t2c, numbers[timeOnes][2]);  digitalWrite(t2d, numbers[timeOnes][3]);  digitalWrite(t2e, numbers[timeOnes][4]);  digitalWrite(t2f, numbers[timeOnes][5]);  digitalWrite(t2g, numbers[timeOnes][6]);
//  digitalWrite(p1a, numbers[pointsHundies][0]);  digitalWrite(p1b, numbers[pointsHundies][1]);  digitalWrite(p1c, numbers[pointsHundies][2]);  digitalWrite(p1d, numbers[pointsHundies][3]);  digitalWrite(p1e, numbers[pointsHundies][4]);  digitalWrite(p1f, numbers[pointsHundies][5]);  digitalWrite(p1g, numbers[pointsHundies][6]);
//  digitalWrite(p2a, numbers[pointsTens][0]);  digitalWrite(p2b, numbers[pointsTens][1]);  digitalWrite(p2c, numbers[pointsTens][2]);  digitalWrite(p2d, numbers[pointsTens][3]);  digitalWrite(p2e, numbers[pointsTens][4]);  digitalWrite(p2f, numbers[pointsTens][5]);  digitalWrite(p2g, numbers[pointsTens][6]);
//  digitalWrite(p3a, numbers[pointsOnes][0]);  digitalWrite(p3b, numbers[pointsOnes][1]);  digitalWrite(p3c, numbers[pointsOnes][2]);  digitalWrite(p3d, numbers[pointsOnes][3]);  digitalWrite(p3e, numbers[pointsOnes][4]);  digitalWrite(p3f, numbers[pointsOnes][5]);  digitalWrite(p3g, numbers[pointsOnes][6]);
//

/*
  if((currTimeMillis - prevTimeMillis) > 500){
  number = (number + 1) % 1000;

  }
*/
prevSensor = sensor;
prevStartButton = startButton;
prevMultiButton = multiButton;
prevTesterMode = testerMode;

// Print the state of things
if ((currTimeMillis - prevDebugMillis) > 1000) {
  Serial.print("Mode:   "); Serial.println(mode);
  Serial.print("Time:   "); Serial.println(time);
  Serial.print("Points: "); Serial.println(points);
  Serial.println();
  prevDebugMillis = millis();
}

}

void writeDisplay() {
  tlc.setPWM(t1a, numbers[timeTens][0]);  tlc.setPWM(t1b, numbers[timeTens][1]);  tlc.setPWM(t1c, numbers[timeTens][2]);  tlc.setPWM(t1d, numbers[timeTens][3]);  tlc.setPWM(t1e, numbers[timeTens][4]);  tlc.setPWM(t1f, numbers[timeTens][5]);  tlc.setPWM(t1g, numbers[timeTens][6]);
  tlc.setPWM(t2a, numbers[timeOnes][0]);  tlc.setPWM(t2b, numbers[timeOnes][1]);  tlc.setPWM(t2c, numbers[timeOnes][2]);  tlc.setPWM(t2d, numbers[timeOnes][3]);  tlc.setPWM(t2e, numbers[timeOnes][4]);  tlc.setPWM(t2f, numbers[timeOnes][5]);  tlc.setPWM(t2g, numbers[timeOnes][6]);
  tlc.setPWM(p1a, numbers[pointsHundies][0]);  tlc.setPWM(p1b, numbers[pointsHundies][1]);  tlc.setPWM(p1c, numbers[pointsHundies][2]);  tlc.setPWM(p1d, numbers[pointsHundies][3]);  tlc.setPWM(p1e, numbers[pointsHundies][4]);  tlc.setPWM(p1f, numbers[pointsHundies][5]);  tlc.setPWM(p1g, numbers[pointsHundies][6]);
  tlc.setPWM(p2a, numbers[pointsTens][0]);  tlc.setPWM(p2b, numbers[pointsTens][1]);  tlc.setPWM(p2c, numbers[pointsTens][2]);  tlc.setPWM(p2d, numbers[pointsTens][3]);  tlc.setPWM(p2e, numbers[pointsTens][4]);  tlc.setPWM(p2f, numbers[pointsTens][5]);  tlc.setPWM(p2g, numbers[pointsTens][6]);
  tlc.setPWM(p3a, numbers[pointsOnes][0]);  tlc.setPWM(p3b, numbers[pointsOnes][1]);  tlc.setPWM(p3c, numbers[pointsOnes][2]);  tlc.setPWM(p3d, numbers[pointsOnes][3]);  tlc.setPWM(p3e, numbers[pointsOnes][4]);  tlc.setPWM(p3f, numbers[pointsOnes][5]);  tlc.setPWM(p3g, numbers[pointsOnes][6]);
  tlc.write();
}
