class Hoop
{
  public:

    Adafruit_TLC5947* tlc;
    unsigned long lastDisplayUpdate;
    int refreshRate;

    SevenSeg scoreDisplay;
    SevenSeg timerDisplay;

    int firstDisplayPin;
    int scoreDisplayPin;
    int timerDisplayPin;

    boolean gamePlaying = false;

    int lane;
    int points;
    int secondsRemaining;
    int gameLength = 45; // Seconds
    int millisBeforeScoreClears = 10 * 60 * 60 * 60 * 1000; // 10 min
    int effectStep;

    int sensorPin;
    int startButtonPin;
    int multiButtonPin;
    int startButtonLight;
    int multiButtonLight;

    int clockPin;
    int dataPin;
    int latchPin;

    Bounce startButton;
    Bounce multiButton;
    Bounce sensorInput;

    int sensor; // depreciated
    int lastSensorState; // depreciated

    unsigned long currTime;
    unsigned long lastTimerUpdate;
    unsigned long lastGameOver;
    unsigned long lastEffectUpdate;
    unsigned long shotStartTime;
    unsigned long lastScoreTime;
    unsigned long sensorOnTime;
    unsigned long sinceLastPointTime;
    unsigned long lastStartButtonTime;
    unsigned long lastTimerFlashTime;
    bool lastTimerFlashState;
    bool gamePaused = false;


    Hoop(int l, int cp, int dp, int lp, int sp, int sbp, int sbl, int mbp, int mbl)
    {
      lane = l;
      sensorPin = sp;
      startButtonPin = sbp;
      multiButtonPin = mbp;
      startButtonLight = sbl;
      multiButtonLight = mbl;
      clockPin = cp;
      dataPin = dp;
      latchPin = lp;
    }

    void Setup()
    {
      tlc = new Adafruit_TLC5947(2, clockPin, dataPin, latchPin, -1);
      scoreDisplay.Setup(3, 0);
      timerDisplay.Setup(2, 24);

      scoreDisplay.ON = 800; // Max brightness of score
      timerDisplay.ON = 3400; // Max brightness of time
      refreshRate = 100;

      timerDisplay.Set(0);
      scoreDisplay.Set(0);

      pinMode(sensorPin, INPUT);
      //sensorInput.attach(sensorPin);
      //sensorInput.interval(40);

      pinMode(startButtonPin, INPUT_PULLUP);
      startButton.attach(startButtonPin);
      startButton.interval(5);

      pinMode(multiButtonPin, INPUT_PULLUP);
      multiButton.attach(multiButtonPin);
      multiButton.interval(5);

      pinMode(startButtonLight, OUTPUT);
      pinMode(multiButtonLight, OUTPUT);
      digitalWrite(startButtonLight, HIGH);
      digitalWrite(multiButtonLight, LOW); // Currently does nothing

      tlc->begin();

      PrintStatus();

      //scoreDisplay.PrintStatus();
      //timerDisplay.PrintStatus();
      EndGame();
    }

    void Update()
    {
      currTime = millis();
      CheckButtons();

      if (!gamePaused) {

        if (gamePlaying) {

          UpdateTimer();
          CheckHoopSensor();

        } else {

          if ((currTime - lastGameOver) > millisBeforeScoreClears) {
            // Start attractor effect 10 min after game ends
            UpdateEffect();
          } else if ((currTime - lastGameOver) < 5000) {
            // Flash the timer for 5 seconds after game ends
            FlashTimer();
          } else {
            // Keep last score up
            // Keep timer on zero
            timerDisplay.Set(0);
          }

        }
      }
    }

    void StartGame()
    {
      gamePlaying = true;

      secondsRemaining = gameLength;
      timerDisplay.Set(gameLength);

      points = 0;
      scoreDisplay.Set(points);

      lastScoreTime = 0;
      sensorOnTime = 0;
      shotStartTime = 0;
      sinceLastPointTime = 0;
      lastSensorState = HIGH; // DEFAULT, UNTRIGGERED


      Serial.println((String)"Hoop " + lane + ": GAME STARTED!");
    }

    void EndGame()
    {
      gamePlaying = false;
      lastGameOver = currTime;
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
      Serial.println((String)"Hoop " + lane + " NEW SCORE: " + points);
    }

    void UpdateTimer()
    {

      if ((millis() - lastTimerUpdate) > 1000)
      {
        timerDisplay.Set(secondsRemaining);

        // Give the player a whole second at zero
        if (secondsRemaining <= 0)
        {
          EndGame();
        } else {
          secondsRemaining -= 1;
        }

        lastTimerUpdate = millis();

        Serial.println((String)"Hoop " + lane + " time: " + secondsRemaining);
      }
    }

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
        disp.pins[(i + 1) % disp.numPins] = disp.ON / 10;
        disp.pins[(i + 2) % disp.numPins] = disp.ON;
        disp.pins[(i + 3) % disp.numPins] = disp.ON;
        disp.pins[(i + 4) % disp.numPins] = disp.ON;
        disp.pins[(i + 5) % disp.numPins] = disp.ON;
        disp.pins[(i + 6) % disp.numPins] = disp.ON;
        disp.pins[(i + 7) % disp.numPins] = disp.ON;
        disp.pins[(i + 8) % disp.numPins] = disp.ON;
        disp.pins[(i + 9) % disp.numPins] = disp.ON;

        timerDisplay.Set(0);
        //        // Flash 77 on Timer
        //        if ((effectStep % 10) > 5) {
        //          timerDisplay.SetAllSegs(0);
        //        } else {
        //          timerDisplay.Set(77);
        //        }

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
      //Serial.println((String)"  tlc ready: " + tlc->status);
      Serial.println();
    }

    void CheckHoopSensorDebounce() {
      sensorInput.update();

      int sensorState = sensorInput.read();

      if (sensorInput.fell()) {
        AddPoints();
        Serial.println((String)"SENSOR " + sensorState + " time: " + millis());
      }
      if (sensorInput.rose()) {
        Serial.println((String)"SENSOR " + sensorState + " time: " + millis());
      }
    }

    void CheckHoopSensor()
    {
      sensor = digitalRead(sensorPin);
      //Serial.println((String)"Hoop " + lane + " sensor: " + sensor);

      if (sensor == LOW && lastSensorState == HIGH) {
        shotStartTime = currTime;
        lastSensorState = sensor;
        Serial.println((String)"Hoop " + lane + " shot started");
      }

      if (sensor == HIGH && lastSensorState == LOW) {
        sensorOnTime = currTime - shotStartTime;
        sinceLastPointTime = currTime - lastScoreTime;
        lastSensorState = sensor;
        Serial.println((String)"Hoop " + lane + " sensor on time: " + sensorOnTime);
        Serial.println((String)"Hoop " + lane + " since last point: " + sinceLastPointTime);

        /* 
         *  HOOP SCORE SENSOR CAN BE ADJUSTED HERE
         */
         
        if (sensorOnTime > 5 && sensorOnTime < 400) {
          // Looks like a basketball passing through, not net.

          if (sinceLastPointTime > 400) {
            // Minimum realistic time between shots

            AddPoints();

            lastScoreTime = currTime;
          }
        }
        sensorOnTime = 0;
        shotStartTime = 0;
        sinceLastPointTime = 0;
      }
    }

    void CheckButtons()
    {
      startButton.update();
      multiButton.update();

      int startButtonState = startButton.read();
      int multiButtonState = multiButton.read();

      digitalWrite(startButtonLight, startButtonState);
      // Multi button should be off by default
      digitalWrite(multiButtonLight, !multiButtonState);

      if (startButton.fell()) {
        // Prepare game for before button is released
        EndGame();
        timerDisplay.Set(88);
        scoreDisplay.Set(888);
        gamePaused = true;
      }

      if (startButton.rose()) {
        StartGame();
        gamePaused = false;
      }
    }

    void FlashTimer() {
      if (currTime - lastTimerFlashTime > 500) {
        if (lastTimerFlashState == false) {
          timerDisplay.Set(0);
        } else {
          timerDisplay.SetAllSegs(0);
        }
        lastTimerFlashTime = currTime;
        lastTimerFlashState = !lastTimerFlashState;
      }
    }

    void UpdateDisplay()
    {

      for (int i = 0; i < timerDisplay.numPins; i++)
      {
        int val = timerDisplay.pins[i];
        int pin = i + timerDisplay.firstPin;
        tlc->setPWM(pin, val);
        //Serial.println((String)"pin "+pin+" val "+val);
      }

      for (int i = 0; i < scoreDisplay.numPins; i++)
      {
        int val = scoreDisplay.pins[i];
        int pin = i + scoreDisplay.firstPin;
        tlc->setPWM(pin, val);
        //Serial.println((String)"pin "+pin+" val "+val);
      }

      if (currTime - lastDisplayUpdate > refreshRate)
      {
        // if buffer has changed?
        tlc->write();
        lastDisplayUpdate = currTime;
      }

    }
};
