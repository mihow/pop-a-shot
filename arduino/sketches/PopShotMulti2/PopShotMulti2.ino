#include "Servo.h"
#include "Adafruit_TLC5947.h"
#include "Bounce2.h"
 
class Display
{
	// Class Member Variables
	// These are initialized at startup
	int ledPin;      // the number of the LED pin
	long OnTime;     // milliseconds of on-time
	long OffTime;    // milliseconds of off-time
 
	// These maintain the current state
	int ledState;             		// ledState used to set the LED
	unsigned long previousMillis;  	// will store last time LED was updated
 
  // Constructor - creates a Display 
  // and initializes the member variables and state
  public:
  Display(int pin, long on, long off)
  {
	ledPin = pin;
	pinMode(ledPin, OUTPUT);     
	  
	OnTime = on;
	OffTime = off;
	
	ledState = LOW; 
	previousMillis = 0;
  }
 
  void Update()
  {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
     
    if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
      ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(ledPin, ledState);  // Update the actual LED
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(ledPin, ledState);	  // Update the actual LED
    }
  }
};
 
class Hoop
{
  //Display scoreDisplay;
  //Display timeDisplay;

  boolean gamePlaying = false;

  int lane;
  int points;
  int secondsRemaining;
  int gameLength = 45;

  unsigned long lastTimerUpdate;
 
public: 
  Hoop(int l)
  {
    lane = l;
  }
  
  void Setup()
  {
    pinMode(13, OUTPUT); 
  }
  
  void Update()
  {
    if(gamePlaying) {

      UpdateTimer();
      CheckHoopSensor();

    } else {

      CheckStartButton();
      CheckMultiButton();

    }
  }

  void StartGame()
  {
    gamePlaying = true;  
    points = 0;
    secondsRemaining = gameLength;
    Serial.println((String)"Hoop " + lane + ": GAME STARTED!");
  }

  void EndGame()
  {
    gamePlaying = false;  
    Serial.println((String)"Hoop " + lane + ": GAME OVER!");
    PrintStatus();
  }

  void AddPoints()
  {
    if( secondsRemaining <= 10 )
    {
      points += 3;
    } else {
      points += 2;
    }
    Serial.println((String)"Hoop " + lane + " points: " + points);
  }

  void UpdateTimer()
  {
    if((millis() - lastTimerUpdate) > 1000)
    {
      secondsRemaining -= 1;
      lastTimerUpdate = millis();
      Serial.println((String)"Hoop " + lane + " time: " + secondsRemaining);
    }
    if(secondsRemaining <= 0)
    {
      EndGame(); 
    }
  }

  void PrintStatus()
  {
    Serial.println();
    Serial.println((String)"Hoop " + lane + "summary:");
    Serial.println((String)"  playing: " + gamePlaying);
    Serial.println((String)"  time: " + secondsRemaining);
    Serial.println((String)"  score: " + points);
    Serial.println();
  }

  void CheckHoopSensor()
  {
    if(false)
    {
      AddPoints(); 
    }
  }

  void CheckStartButton()
  {
  }

  void CheckMultiButton()
  {
  }
};
 
 
Hoop hoop1(1);
Hoop hoop2(2);

char serialInput;
 
void setup() 
{ 
  Serial.begin(9600);
  hoop1.Setup();
  hoop2.Setup();
} 
 
 
void loop() 
{ 
  readSerial();
  hoop1.Update();
  hoop2.Update();
}


void readSerial() {
  if (Serial.available() > 0) {
      serialInput = Serial.read();
      switch ( serialInput ) {
        case 'S':
          hoop1.StartGame();
          break;
        case 'P':
          hoop1.AddPoints();
          break;
        case 'A':
          hoop2.StartGame();
          break;
        case 'O':
          hoop2.AddPoints();
          break;
        default:
          Serial.print("What? Received: ");
          Serial.println(serialInput);
          break;
      }
  }
}
