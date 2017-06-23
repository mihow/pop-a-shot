#include "Servo.h"
#include "Adafruit_TLC5947.h"
#include "Bounce2.h"
 
class Display
{
	int numDigits;
    int firstPin;
    int *pins;
    int pinsPerDigit = 8;
    
    int ON = 4095;
    int OFF = 0;
 
	unsigned long previousMillis;
 
  public:
  int numPins;

  Display()
  {
	previousMillis = 0;
  }
 
  void Setup(int nd)
  {
	numDigits = nd;
    numPins = numDigits * pinsPerDigit;
    //int *pins = new int[numPins]();

    // https://arduino.stackexchange.com/questions/3774/how-can-i-declare-an-array-of-variable-size-globally
	// if (pins != 0) {
	// 	pins = (int*) realloc(pins, numPins * sizeof(int));
	// } else {
	// 	pins = (int*) malloc(numPins * sizeof(int));
	// }
    // Allocate array of pin values they way Adafruit_TLC5947 does
    pins = (int *)malloc(numPins * sizeof(int));
    memset(pins, 0, numPins * sizeof(int));

    //Serial.println((String)"Pins size: " + sizeof(pins));
    Clear();
  }

  void PrintStatus()
  {
    for(int i=0; i<numPins; i++) {
      Serial.print((String)"Pin " + i + ": " + pins[i] + ", ");
    }
    Serial.println();
  }

  void Clear()
  {
    for(int i=0; i<numPins; i++) {
      pins[i] = OFF;
    }
  }

  void SetPin(int pin, int val)
  {
    pins[pin] = val;
  }

  int GetPin(int pin)
  {
    int val = pins[pin];
    Serial.println((String)"Pin " + pin + ": " + val);
  }

  void Update()
  {
    unsigned long currentMillis = millis();
     
    previousMillis = currentMillis;
  }
};
 
class Hoop
{
  Display scoreDisplay;
  Display timerDisplay;

  boolean gamePlaying = false;

  int lane;
  int points;
  int secondsRemaining;
  int gameLength = 45;

  int firstDisplayPin;
  int sensorPin;
  int startButtonPin;
  int multiButtonPin;
  int startButtonLight;
  int multiButtonLight;

  unsigned long lastTimerUpdate;
 
public: 
  Hoop(int l, int fp)
  {
    lane = l;
    firstDisplayPin = fp;
    // sensorPin = sp;
    // startButtonPin = sbp;
    // multiButtonPin = mbp;
    // startButtonLight = sbl;
    // multiButtonLight = mbl;
  }
  
  void Setup()
  {
    pinMode(13, OUTPUT); 

    scoreDisplay.Setup(3);
    timerDisplay.Setup(2);

    scoreDisplay.SetPin(5, 555);
    timerDisplay.SetPin(8, 888);

    PrintStatus();
    scoreDisplay.PrintStatus();
    timerDisplay.PrintStatus();
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
    Serial.println((String)"Hoop " + lane + " summary:");
    Serial.println((String)"  playing: " + gamePlaying);
    Serial.println((String)"  time: " + secondsRemaining);
    Serial.println((String)"  score: " + points);
    Serial.println((String)"  num score pins: " + scoreDisplay.numPins);
    Serial.println((String)"  num timer pins: " + timerDisplay.numPins);
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
 
 
Hoop hoop1(1, 0);
Hoop hoop2(2, 49);

char serialInput;
 
void setup() 
{ 
  Serial.begin(9600);
  hoop1.Setup();
  hoop2.Setup();
  Serial.println("Ready!");
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
        case 'q':
          hoop1.StartGame();
          break;
        case 'w':
          hoop1.AddPoints();
          break;
        case 'e':
          hoop1.PrintStatus();
          break;
        case 'a':
          hoop2.StartGame();
          break;
        case 's':
          hoop2.AddPoints();
          break;
        case 'd':
          hoop1.PrintStatus();
          break;
        default:
          Serial.print("What? Received: ");
          Serial.println(serialInput);
          break;
      }
  }
}
