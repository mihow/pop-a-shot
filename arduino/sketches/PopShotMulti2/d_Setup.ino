Adafruit_TLC5947 tlc = Adafruit_TLC5947(2, 45, 46, 47);


Hoop hoop1(
  4, // Lane number
  0, // First display pin
  51, // Sensor pin (CHANGE TO 58)
  41, // Start button pin
  40, // Multi button pin
  8, // Start button light
  7 // Multi button light
);

Hoop hoop3(
  3, // Lane number
  49, // First display pin
  59, // Sensor pin
  38, // Start button pin
  37, // Multi button pin
  6, // Start button light
  9 // Multi button light
);

Hoop hoop2(
  2, // Lane number
  73, // First display pin
  57, // Sensor pin
  999, // Start button pin
  999, // Multi button pin
  999, // Start button light
  999 // Multi button light
);

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
