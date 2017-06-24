class Hoop
{
  public:

    SevenSeg scoreDisplay;
    SevenSeg timerDisplay;

    int firstDisplayPin;
    int scoreDisplayPin;
    int timerDisplayPin;

    boolean gamePlaying = false;

    int lane;
    int points;
    int secondsRemaining;
    int gameLength = 45;

    int sensorPin;
    int startButtonPin;
    int multiButtonPin;
    int startButtonLight;
    int multiButtonLight;

    unsigned long lastTimerUpdate;
    unsigned long lastGameOver;
    unsigned long lastEffectUpdate;
    int effectStep;

    Hoop(int l, int fp)
    {
      lane = l;
      firstDisplayPin = fp;
      scoreDisplayPin = fp;
      timerDisplayPin = fp + 24; // Offset by 3 digits.
      // sensorPin = sp;
      // startButtonPin = sbp;
      // multiButtonPin = mbp;
      // startButtonLight = sbl;
      // multiButtonLight = mbl;
    }

    void Setup()
    {
      pinMode(13, OUTPUT);

      scoreDisplay.Setup(3, scoreDisplayPin);
      timerDisplay.Setup(2, timerDisplayPin);

      // scoreDisplay.ON = 100; // Lower brightness of score

      timerDisplay.Set(0);
      scoreDisplay.Set(0);

      PrintStatus();

      //scoreDisplay.PrintStatus();
      //timerDisplay.PrintStatus();
    }

    void Update()
    {
      if (gamePlaying) {

        UpdateTimer();
        CheckHoopSensor();

      } else {

        CheckStartButton();
        CheckMultiButton();

        if (millis() - lastGameOver > 60000) {
          // Start attractor effect 1 min after game ends
          UpdateEffect();
        } else {
          // Flash last score
        }

      }
    }

    void StartGame()
    {
      gamePlaying = true;
      points = 0;
      secondsRemaining = gameLength;
      scoreDisplay.Set(points);
      Serial.println((String)"Hoop " + lane + ": GAME STARTED!");
    }

    void EndGame()
    {
      gamePlaying = false;
      lastGameOver = millis();
      Serial.println((String)"Hoop " + lane + ": GAME OVER!");
    }

    void AddPoints()
    {
      if ( secondsRemaining <= 10 )
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
      if ((millis() - lastTimerUpdate) > 1000)
      {
        secondsRemaining -= 1;
        lastTimerUpdate = millis();
        timerDisplay.Set(secondsRemaining);
        Serial.println((String)"Hoop " + lane + " time: " + secondsRemaining);
      }
      if (secondsRemaining <= 0)
      {
        EndGame();
      }
    }

    //    void UpdateEffect()
    //    {
    //      if ((millis() - lastEffectUpdate) > 1000)
    //      {
    //        if (effectStep == 0)
    //        {
    //          timerDisplay.Set(77);
    //          scoreDisplay.SetAllSegs(0);
    //          effectStep = 1;
    //        } else {
    //          timerDisplay.SetAllSegs(0);
    //          scoreDisplay.Set(777);
    //          effectState = 0;
    //        }
    //        lastEffectUpdate = millis();
    //      }
    //    }

    void UpdateEffect()
    {
      SevenSeg disp = scoreDisplay;
      int numSteps = disp.numPins;

      // Snake through Score pins
      if ((millis() - lastEffectUpdate) > 180) {
        disp.SetAllSegs(0);
        // int seg = effectStep % disp.numSegments;
        // int digit = effectStep % disp.numDigits;
        int i = effectStep % disp.numPins;
        disp.pins[i] = disp.ON / 100;
        disp.pins[(i+1) % disp.numPins] = disp.ON / 10;
        disp.pins[(i+2) % disp.numPins] = disp.ON;
        disp.pins[(i+3) % disp.numPins] = disp.ON;
        disp.pins[(i+4) % disp.numPins] = disp.ON;
        disp.pins[(i+5) % disp.numPins] = disp.ON;

        // Flash 77 on Timer
        if ((effectStep % 10) > 5) {
          timerDisplay.SetAllSegs(0);
        } else {
          timerDisplay.Set(77);
        }

        effectStep++;

        if ( effectStep > numSteps ) {
          effectStep = 0;
        }

        lastEffectUpdate = millis();
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
      if (false)
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

    void UpdateDisplay(Adafruit_TLC5947 tlc)
    {

      for (int i = 0; i < timerDisplay.numPins; i++)
      {
        int val = timerDisplay.pins[i];
        int pin = i + timerDisplay.firstPin;
        tlc.setPWM(pin, val);
        //Serial.println((String)"pin "+pin+" val "+val);
      }

      for (int i = 0; i < scoreDisplay.numPins; i++)
      {
        int val = scoreDisplay.pins[i];
        int pin = i + scoreDisplay.firstPin;
        tlc.setPWM(pin, val);
        //Serial.println((String)"pin "+pin+" val "+val);
      }

    }
};
