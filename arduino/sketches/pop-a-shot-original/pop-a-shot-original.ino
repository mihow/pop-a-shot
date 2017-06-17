int number = 0;
int timeTens,timeOnes,pointsHundies,pointsTens,pointsOnes;
int time = 0;
int startTime = 45;
int points = 0;
int currTimeMillis, timeMillis, prevTimeMillis, prevTesterClockMillis;
int scoreMillis, prevScoreMillis;
int startButton,prevStartButton, multiButton,prevMultiButton, sensor,prevSensor;
int testerMode, prevTesterMode;
int hoop4Multi, hoop3Multi, hoop2Multi, hoop1Multi;
int someoneElsePressedMulti;
int timeBetweenShots = 300;
int testerClock = 0;

int mode = 0; //0 = waiting, 1= start button pressed, 2 = playing, 3 = game over, 4 = you have pressed MULTI, 5 = someone else has pressed MULTI,
              //6 = you and someone else have pressed multi, 7 = you have released multi but someone else is still holding it
              //8 = you are the last one holding multi 9 = tester mode

              
int numbers[10][7] = {{1,1,1,1,1,1,0},//zero
                      {0,1,1,0,0,0,0},//one
                      {1,1,0,1,1,0,1},//two
                      {1,1,1,1,0,0,1},//three
                      {0,1,1,0,0,1,1},//four
                      {1,0,1,1,0,1,1},//five
                      {1,0,1,1,1,1,1},//six
                      {1,1,1,0,0,0,0},//seven
                      {1,1,1,1,1,1,1},//eight
                      {1,1,1,0,0,1,1}};//niner



int startButtonPin = 5;//NOTE THIS IS LOW=TRUE
int multiButtonPin = 6;//NOTE THIS IS LOW=TRUE
int sensorPin = 2; //HOOOOOOOOOOP                    
int testerModePin = 3;
int multiPin = 41;      //THIS WIRE COLOR SHOULD MATCH THE HOOPS MULTIPIN WIRE NUMBER BELOW
int hoop1MultiPin = 43;// on the arduino, ground the hoops own multiPin          //RED  WIRE
int hoop2MultiPin = 45;// e.g. on hoop 4's arduino, ground pin49.                //GREEN WIRE
int hoop3MultiPin = 47;//then connect pin multiPin(41) to other hoops' pin 49.   //BLUE WIRE
int hoop4MultiPin = 49;                                                          //PURPLE WIRE
int t1a =  13;int t1b =  12;int t1c =  11;int t1d =  10;int t1e = 9;int t1f =  8;int t1g =  7;//time tens
int t2a = 14;int t2b = 15;int t2c = 16;int t2d = 17;int t2e = 18;int t2f = 19;int t2g = 20;//time ones
int p1a = 22;int p1b = 24;int p1c = 26;int p1d = 28;int p1e = 30;int p1f = 32;int p1g = 34;//points hundies
int p2a = 23;int p2b = 25;int p2c = 27;int p2d = 29;int p2e = 31;int p2f = 33;int p2g = 35;//points tens
int p3a = 36;int p3b = 38;int p3c = 40;int p3d = 42;int p3e = 44;int p3f = 46;int p3g = 48;//points ones


                   
void setup(){

pinMode(startButtonPin, INPUT); pinMode(multiButtonPin, INPUT); pinMode(sensorPin, INPUT);
pinMode(multiPin, OUTPUT);
pinMode(hoop1MultiPin, INPUT);pinMode(hoop2MultiPin, INPUT);pinMode(hoop3MultiPin, INPUT);pinMode(hoop4MultiPin, INPUT);
pinMode(t1a, OUTPUT);pinMode(t1b, OUTPUT);pinMode(t1c, OUTPUT);pinMode(t1d, OUTPUT);pinMode(t1e, OUTPUT);pinMode(t1f, OUTPUT);pinMode(t1g, OUTPUT);
pinMode(t2a, OUTPUT);pinMode(t2b, OUTPUT);pinMode(t2c, OUTPUT);pinMode(t2d, OUTPUT);pinMode(t2e, OUTPUT);pinMode(t2f, OUTPUT);pinMode(t2g, OUTPUT);
pinMode(p1a, OUTPUT);pinMode(p1b, OUTPUT);pinMode(p1c, OUTPUT);pinMode(p1d, OUTPUT);pinMode(p1e, OUTPUT);pinMode(p1f, OUTPUT);pinMode(p1g, OUTPUT);
pinMode(p2a, OUTPUT);pinMode(p2b, OUTPUT);pinMode(p2c, OUTPUT);pinMode(p2d, OUTPUT);pinMode(p2e, OUTPUT);pinMode(p2f, OUTPUT);pinMode(p2g, OUTPUT);
pinMode(p3a, OUTPUT);pinMode(p3b, OUTPUT);pinMode(p3c, OUTPUT);pinMode(p3d, OUTPUT);pinMode(p3e, OUTPUT);pinMode(p3f, OUTPUT);pinMode(p3g, OUTPUT);

digitalWrite(multiPin, LOW);

  
}

void loop(){
  currTimeMillis = millis();
  startButton = digitalRead(startButtonPin);
  multiButton = digitalRead(multiButtonPin);
  sensor = digitalRead(sensorPin);
  hoop1Multi = digitalRead(hoop1MultiPin);
  hoop2Multi = digitalRead(hoop2MultiPin);
  hoop3Multi = digitalRead(hoop3MultiPin);
  hoop4Multi = digitalRead(hoop4MultiPin);
  testerMode = digitalRead(testerModePin);
  
 if(multiButton == 0){
   digitalWrite(multiPin, HIGH);
 }else{
   digitalWrite(multiPin, LOW);
 }
 if((hoop1Multi + hoop2Multi + hoop3Multi + hoop4Multi) > 0){
   someoneElsePressedMulti = 1;
 }else{
   someoneElsePressedMulti = 0;
 } 
 
 ///////WAITING
 if(mode == 0){
   if(startButton == 0){
     prevTimeMillis = currTimeMillis;
     mode = 1;
   }
   if(multiButton == 0){
     prevTimeMillis = currTimeMillis;
     mode = 4;
   }
   if(someoneElsePressedMulti == 1){
     mode = 5;
     prevTimeMillis = currTimeMillis;
   }
   if(testerMode == 1){
      if(prevTesterMode == 0){
     prevTesterClockMillis = currTimeMillis;
     prevTimeMillis = currTimeMillis;
     mode = 9;
     }
   }
 } 
 
 //////START BUTTON PRESSED
 if(mode == 1){
     if(startButton == 1){
       if((currTimeMillis - prevTimeMillis) > 100){///have this on every change of state that involves a button in order to debounce
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
if(mode == 2){
    if(sensor != prevSensor){
      if(sensor == 1){
        scoreMillis = currTimeMillis;
          if((scoreMillis - prevScoreMillis) > 500){
            prevScoreMillis = scoreMillis;
            if(time > 10){
               points += 2;
        }else{
          points += 3;
        }
      }
    }
    }

    
    if((currTimeMillis - prevTimeMillis) > 1000){
        prevTimeMillis = currTimeMillis;
      if(time > 1){
        time -= 1;
      }else{
        time = 0;
        mode = 3;
      }
    }
}
     
  
///////////////GAME OVER
    if(mode == 3){
      if((currTimeMillis - prevTimeMillis) > 1000){
         prevTimeMillis = currTimeMillis; 
         mode = 0;
      }
    }
    

////YOU PRESSED MULTI
     if(mode == 4){
       if((currTimeMillis - prevTimeMillis) > 100){
         if(multiButton == 1){
           mode = 0;
         }
         if(someoneElsePressedMulti == 1){
           mode = 6;
         }
       }
     }
///////////SOMEONE ELSE PRESSED MULTI, BUT YOU DIDN'T
      if(mode == 5){
        if(someoneElsePressedMulti == 0){//resets if other people let go of multi before you press it
          mode = 0;                       //also note that this isn't debounced because this signal is coming from other arduino digi-outs
        }
        if(multiButton == 0){
          prevTimeMillis = currTimeMillis;
          mode = 6;
      }
      }
      
/////////////YOU AND SOMEONE ELSE ARE PRESSING MULTI
      if(mode == 6){
        time = 77;
        points == 77;
         if((currTimeMillis - prevTimeMillis) > 100){
        if(someoneElsePressedMulti == 0){//again, not debounced because this comes from another arduino
        prevTimeMillis = currTimeMillis;
        mode = 8;
        }
          if(multiButton == 1){
            mode = 7;
          }
        }
      }
 /////////////////YOU HAVE RELEASED MULTI BUT SOMEONE ELSE IS STILL HOLDING IT
       if(mode == 7){
         if(someoneElsePressedMulti == 0){
           prevTimeMillis = currTimeMillis;
           time = startTime;
       points = 0;
       prevScoreMillis = currTimeMillis;
           mode = 2;
         }
       }
       
///////////////YOU ARE THE LAST ONE HOLDING THE MULTI BUTTON
       if(mode == 8){
         if((currTimeMillis - prevTimeMillis) > 100){
           if(multiButton == 1){
             prevTimeMillis = currTimeMillis;
             time = startTime;
             points = 0;
             prevScoreMillis = currTimeMillis;
             mode = 2;
           }
         }
       }
       
/////////////////TESTER MODE
    if(mode == 9){
      timeOnes = testerClock;
      timeTens = testerClock;
      pointsOnes = testerClock;
      pointsTens = testerClock;
      pointsHundies = testerClock;
      if(currTimeMillis - prevTesterClockMillis > 500){
        prevTesterClockMillis = currTimeMillis;
        testerClock = (testerClock + 1) % 10;
      }
      if(testerMode == 1){
        if(prevTesterMode == 0){
          if(currTimeMillis - prevTimeMillis > 500){
        mode = 0;
          }
      }
      }
    }

 
 
if(mode != 9){
  timeOnes = time % 10;
  timeTens = (time - timeOnes) / 10;

  pointsOnes = points % 10;
  pointsTens = ((points - pointsOnes) / 10) % 10;
  pointsHundies = ((points - (pointsTens + pointsOnes)) / 100) % 10;
}
/*
//the following was very useful for debugging, just comment out the above lines that define points and
//the points displays can be used to show mode, multiBUtton etc..
  pointsOnes = mode%10;
  pointsTens = multiButton;
  pointsHundies = someoneElsePressedMulti;
*/
  

  digitalWrite(t1a, numbers[timeTens][0]);  digitalWrite(t1b, numbers[timeTens][1]);  digitalWrite(t1c, numbers[timeTens][2]);  digitalWrite(t1d, numbers[timeTens][3]);  digitalWrite(t1e, numbers[timeTens][4]);  digitalWrite(t1f, numbers[timeTens][5]);  digitalWrite(t1g, numbers[timeTens][6]);
  digitalWrite(t2a, numbers[timeOnes][0]);  digitalWrite(t2b, numbers[timeOnes][1]);  digitalWrite(t2c, numbers[timeOnes][2]);  digitalWrite(t2d, numbers[timeOnes][3]);  digitalWrite(t2e, numbers[timeOnes][4]);  digitalWrite(t2f, numbers[timeOnes][5]);  digitalWrite(t2g, numbers[timeOnes][6]);
  digitalWrite(p1a, numbers[pointsHundies][0]);  digitalWrite(p1b, numbers[pointsHundies][1]);  digitalWrite(p1c, numbers[pointsHundies][2]);  digitalWrite(p1d, numbers[pointsHundies][3]);  digitalWrite(p1e, numbers[pointsHundies][4]);  digitalWrite(p1f, numbers[pointsHundies][5]);  digitalWrite(p1g, numbers[pointsHundies][6]);
  digitalWrite(p2a, numbers[pointsTens][0]);  digitalWrite(p2b, numbers[pointsTens][1]);  digitalWrite(p2c, numbers[pointsTens][2]);  digitalWrite(p2d, numbers[pointsTens][3]);  digitalWrite(p2e, numbers[pointsTens][4]);  digitalWrite(p2f, numbers[pointsTens][5]);  digitalWrite(p2g, numbers[pointsTens][6]);
  digitalWrite(p3a, numbers[pointsOnes][0]);  digitalWrite(p3b, numbers[pointsOnes][1]);  digitalWrite(p3c, numbers[pointsOnes][2]);  digitalWrite(p3d, numbers[pointsOnes][3]);  digitalWrite(p3e, numbers[pointsOnes][4]);  digitalWrite(p3f, numbers[pointsOnes][5]);  digitalWrite(p3g, numbers[pointsOnes][6]);
  /*
  if((currTimeMillis - prevTimeMillis) > 500){
  number = (number + 1) % 1000;

}
  */ 
  prevSensor = sensor;
  prevStartButton = startButton;
  prevMultiButton = multiButton;
  prevTesterMode = testerMode;

//  delay(1000);
}
