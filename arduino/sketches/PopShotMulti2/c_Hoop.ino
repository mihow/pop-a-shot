class Hoop
{
  public: 

  SevenSeg scoreDisplay;
  SevenSeg timerDisplay;

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

    PrintStatus();
    //scoreDisplay.PrintStatus();
    //timerDisplay.PrintStatus();
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
    scoreDisplay.Set(points);
    Serial.println((String)"Hoop " + lane + " points: " + points);
  }

  void UpdateTimer()
  {
    if((millis() - lastTimerUpdate) > 1000)
    {
      secondsRemaining -= 1;
      lastTimerUpdate = millis();
      timerDisplay.Set(secondsRemaining);
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
