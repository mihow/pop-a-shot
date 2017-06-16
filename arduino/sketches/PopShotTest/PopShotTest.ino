#include <SevenSegTLC.h>

#define NUM_DIGITS 1


SevenSegTLC disp(6, 7, 2, 1, 0, 5, 4);

int sensorPin = 56; // A2 (56-59)
int buttonPin1 = 40; // B4
int buttonPin2 = 41;
int buttonLight1 = 8;
int buttonLight2 = 7;

int sensorValue = LOW;
int buttonState1 = HIGH;
int buttonState2 = HIGH;

int counter = 0;
int score = 0;

long startTime;
long elapsedTime;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonLight1, OUTPUT);
  pinMode(buttonLight2, OUTPUT);

  Serial.begin(9600);
  Serial.println("Setting up display...");
  delay(50);

  disp.setNumDigits(NUM_DIGITS);
  delay(50);

  disp.clearDisp();
  delay(50);

  Serial.println("Ready!");
  startTime = millis();

  disp.write(score);
}

void loop() {

  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  sensorValue = digitalRead(sensorPin);

  elapsedTime = millis() - startTime;

  if (sensorValue == LOW) {
    score++;
    disp.write(score);
    Serial.print("Score: ");
    Serial.println(score);
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    delay(500);
  }

  if (buttonState1 == LOW || buttonState2 == LOW) {
    Serial.println("Button Pressed");
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    disp.cyclePins();
    score = 0;
    disp.write(score);
  }

  digitalWrite(buttonLight1, buttonState1);
  digitalWrite(buttonLight2, buttonState2);
}

