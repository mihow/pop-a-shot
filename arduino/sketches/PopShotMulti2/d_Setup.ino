Adafruit_TLC5947 tlc = Adafruit_TLC5947(8, 45, 46, 47);


Hoop hoop1(
  1, // Lane number
  0, // First display pin
  58, // Sensor pin
  41, // Start button pin
  40, // Multi button pin
  7, // Start button light
  8 // Multi button light
);

Hoop hoop2(
  2, // Lane number
  48, // First display pin
  59, // Sensor pin
  38, // Start button pin
  37, // Multi button pin
  6, // Start button light
  9 // Multi button light
);

Hoop hoop3(
  3, // Lane number
  96, // First display pin
  57, // Sensor pin
  32, // Start button pin
  31, // Multi button pin
  4, // Start button light
  3 // Multi button light
);

Hoop* hoops[] = {&hoop1, &hoop2, &hoop3};
int debugHoop = 0;

char serialInput;
unsigned long lastDisplayUpdate;
int refreshRate = 20;

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

  if (timeNow - lastDisplayUpdate > refreshRate)
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
