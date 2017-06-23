Adafruit_TLC5947 tlc = Adafruit_TLC5947(2, 0, 1, 2);
SevenSeg sevenSeg = SevenSeg(8, 3, 4, 5);

Hoop hoop1(1, 0);
Hoop hoop2(2, 49);

char serialInput;
unsigned long lastDisplayUpdate;
 
void setup() 
{ 
  Serial.begin(9600);
  Serial.println("Setting up...");
  hoop1.Setup();
  hoop2.Setup();
  tlc.begin();
  Serial.println("Ready!");
}

void updateDisplay()
{
  unsigned long timeNow = millis(); 

  // sevenSeg.GetPins(hoop1.score, hoop1.firstScorePin);
  // sevenSeg.GetPins(hoop1.secondsRemaining, hoop1.firstTimerPin);

  // hoop1.updateDisplayBuffer(tlc);
  // hoop2.updateDisplayBuffer(tlc);
  // tlc.write()
  
  if(timeNow - lastDisplayUpdate > 5)
  {
    for(int i=0; i<hoop1.scoreDisplay.numPins; i++) 
    {
      int val = hoop1.scoreDisplay.pins[i];  
      // i + offset
      tlc.setPWM(i, val);
    }
    // if buffer has changed?
    tlc.write();
    lastDisplayUpdate = timeNow; 
  }
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
          hoop2.PrintStatus();
          break;

        default:
          Serial.print("What? Received: ");
          Serial.println(serialInput);
          break;
      }
  }
}
