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
  31, // Start button pin
  32, // Multi button pin
  4, // Start button light
  3 // Multi button light
);

Hoop hoop4(
  4, // Lane number
  144, // First display pin
  56, // Sensor pin // @TODO Wrong pin
  35, // Start button pin // @TODO no light
  34, // Multi button pin
  1, // Start button light
  2 // Multi button light
);

int numHoops = 2;
Hoop* hoops[] = {&hoop1, &hoop3};

int debugHoop = 0;
char serialInput;
unsigned long lastDisplayUpdate;
int refreshRate = 20;
bool gamesPaused = false;


void setup()
{
  Serial.begin(9600);
  Serial.println("Setting up...");

  //hoop1.Setup();

  for (int i = 0; i < numHoops; i++) {
    // Setup all configured hoops
    hoops[i]->Setup();
  }

  tlc.begin();
  Serial.println("Ready!");

//  pinMode(44, OUTPUT);
//  digitalWrite(44, HIGH);

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
        Serial.println((String)"CONTROLLING HOOP " + hoops[debugHoop]->lane);
        break;
      case '2':
        debugHoop = 1;
        Serial.println((String)"CONTROLLING HOOP " + hoops[debugHoop]->lane);
        break;
      case '3':
        debugHoop = 2;
        Serial.println((String)"CONTROLLING HOOP " + hoops[debugHoop]->lane);
        break;
      case '4':
        debugHoop = 3;
        Serial.println((String)"CONTROLLING HOOP " + hoops[debugHoop]->lane);
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

      case 'X':
        writeDisplay();
        break;
      case 'P':
        gamesPaused = !gamesPaused;
        Serial.println((String)"Pause: " + gamesPaused);
        break;

      default:
        Serial.print("What? Received: ");
        Serial.println(serialInput);
        break;
    }
  }
}
