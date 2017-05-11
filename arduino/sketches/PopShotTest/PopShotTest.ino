#include <SevenSegTLC.h>


SevenSegTLC disp(6, 7, 2, 1, 0, 5, 4);

const int buttonPin = 38; 
const int sensorPin = A3;
int buttonState = digitalRead(buttonPin);
int sensorValue = 0;
int counter = 12345;
long startTime;
long elapsedTime;

void setup() {
  pinMode(buttonPin, INPUT);
  
  Serial.begin(9600);
  delay(200);
  Serial.println("Setting up display...");

  disp.setNumDigits(5);
  delay(200);
  
  disp.clearDisp();
  delay(200);

  Serial.println("Ready!");
  startTime = millis();
}

void loop() {  

buttonState = digitalRead(buttonPin);
//sensorValue = analogRead(sensorPin);

elapsedTime = millis() - startTime;
// divide by 1000 to convert to seconds - then cast to an int to print
if (elapsedTime > 10) {
    counter += 1;
    Serial.print( counter );
    Serial.print(" / ");
    startTime = millis();
    Serial.println( (int)(startTime / 1000L));
    // @TODO blink LED
    //digitalWrite(13, HIGH);
    disp.write(counter);
}

if (buttonState == HIGH) {
  counter++;
  //disp.write(counter);
  Serial.println(counter);
  delay(50);
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

