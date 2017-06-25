Adafruit_TLC5947 tlc = Adafruit_TLC5947(2, 45, 46, 47);


Hoop hoop1(
  1, // Lane number
  0, // First display pin
  51, // Sensor pin (CHANGE TO 58)
  41, // Start button pin
  40, // Multi button pin
  13, // Start button light (CHANGE TO 8)
  7 // Multi button light
);

Hoop hoop2(
  2, // Lane number
  49, // First display pin
  59, // Sensor pin
  38, // Start button pin
  37, // Multi button pin
  6, // Start button light
  9 // Multi button light
);

Hoop* hoops[2] = {&hoop1, &hoop2};
int debugHoop = 0;

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

void writeDisplay()
{
  unsigned long timeNow = millis();

  if (timeNow - lastDisplayUpdate > 20)
  {
    // if buffer has changed?
    tlc.write();
    lastDisplayUpdate = timeNow;
  }
}

void readSerial() {
  if (Serial.available() > 0) {
    serialInput = Serial.read();
    
    switch ( serialInput ) {
      case '1':
        debugHoop = 0;
        Serial.println("CONTROLLING HOOP 1");
        break;
      case '2':
        debugHoop = 1;
        Serial.println("CONTROLLING HOOP 2");
        break;

      case 'q':
        hoops[debugHoop]->StartGame();
        break;
      case 'w':
        hoops[debugHoop]->AddPoints();
        break;
      case 'e':
        hoops[debugHoop]->PrintStatus();
        break;

      default:
        Serial.print("What? Received: ");
        Serial.println(serialInput);
        break;
    }
  }
}
