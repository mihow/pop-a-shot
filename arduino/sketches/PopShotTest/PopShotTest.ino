#include <SevenSegTLC.h>


SevenSegTLC disp(6, 7, 2, 1, 0, 5, 4);

const int buttonPin = 38; 
int buttonState = digitalRead(buttonPin);
int counter = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  
  Serial.begin(9600);
  delay(1000);
  
  disp.setNumDigits(2);
  delay(1000);
  
  disp.clearDisp();
  delay(1000);
}

void loop() {  

buttonState = digitalRead(buttonPin);

if (buttonState == HIGH) {
  counter++;
  disp.write(counter);
  delay(1000);
}



//  disp.clearDisp();
//  int randInt = random(11, 99);
//  disp.write(randInt);
//  delay(10);`


//  disp.clearDisp();
//  delay(100);
//  disp.write(8);
//  delay(100);


}

