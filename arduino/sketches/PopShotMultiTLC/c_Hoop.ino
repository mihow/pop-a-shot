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
    int gameLength = 45;
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

    int sensor;
    int lastSensorState;

    unsigned long currTime;
    unsigned long lastTimerUpdate;
    unsigned long lastGameOver;
    unsigned long lastEffectUpdate;
    unsigned long shotStartTime;
    unsigned long lastScoreTime;
    unsigned long sensorOnTime;
    unsigned long lastStartButtonTime;
    unsigned long lastTimerFlashTime;
    bool lastTimerFlashState;


    Hoop(int l, int cp, int dp, int lp, int sp, int sbp, int mbp, int sbl, int mbl)
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
      refreshRate = 20;

      timerDisplay.Set(0);
      scoreDisplay.Set(0);

      pinMode(sensorPin, INPUT);

      pinMode(startButtonPin, INPUT_PULLUP);
      startButton.attach(startButtonPin);
      startButton.interval(5);

      pinMode(multiButtonPin, INPUT_PULLUP);
      multiButton.attach(multiButtonPin);
      multiButton.interval(5);

      pinMode(startButtonLight, OUTPUT);
      pinMode(multiButtonLight, OUTPUT);
      digitalWrite(startButtonLight, HIGH);
      digitalWrite(multiButtonLight, HIGH);

      tlc->begin();
      
      PrintStatus();
      
      //scoreDisplay.PrintStatus();
      //timerDisplay.PrintStatus();
    }

    void Update()
    {
      currTime = millis();

      if (gamePlaying) {

        UpdateTimer();
        CheckHoopSensor();
        CheckButtons(); // @TODO only check when game isn't running?

      } else {

        CheckButtons();

        if (currTime - lastGameOver > 60000) {
          // Start attractor effect 1 min after game ends
          UpdateEffect();
        } else if (currTime - lastGameOver < 5000) {
          // Flash the timer for 10 seconds after game ends
          FlashTimer();
        } else {
          // Keep last score up
        }

      }
    }

    void StartGame()
    {
      gamePlaying = true;

      secondsRemaining = gameLength;
      timerDisplay.Set(55);
      //lastTimerUpdate = millis();

      points = 0;
      scoreDisplay.Set(points);
      lastScoreTime = 0;

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
      Serial.println((String)"Hoop " + lane + " points: " + points);
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

    void CheckHoopSensor()
    {
      sensor = digitalRead(sensorPin);

      if (sensor == LOW && lastSensorState == HIGH) {
        shotStartTime = currTime;
        lastSensorState = LOW;
      }

      if (sensor == HIGH && lastSensorState == LOW) {
        sensorOnTime = currTime - shotStartTime;
        Serial.println((String)"Hoop " + lane + " sensor on time: " + sensorOnTime);

        if (sensorOnTime > 24 && sensorOnTime < 300) {
          // Looks like a basketball passing through, not net.

          if (currTime - lastScoreTime > 500) {
            // Maximum realistic time between shots

            AddPoints();

            lastScoreTime = currTime;
          }
        }
      }
      lastSensorState = sensor;
    }

    void CheckButtons()
    {
      startButton.update();
      multiButton.update();

      int startButtonState = startButton.read();
      int multiButtonState = multiButton.read();

      digitalWrite(startButtonLight, startButtonState);
      digitalWrite(multiButtonLight, multiButtonState);

      if (startButton.fell()) {
        StartGame();
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
