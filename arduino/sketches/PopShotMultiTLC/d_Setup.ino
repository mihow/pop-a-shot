Hoop hoop1(
  1, // Lane number
  42, // Clock
  43, // Data
  18, // Latch // PIN 44 IS DEAD
  56, // Sensor pin
  31, // Start button pin // PIN 32 DEAD?
  2, // Start button light
  31, // Multi button pin // USED FOR START BUTTON
  3 // Multi button light
);

Hoop hoop2(
  2, // Lane number
  45, // Clock
  46, // Data
  47, // Latch
  57, // Sensor pin
  33, // Start button pin // PIN 33 DEAD?
  4, // Start button light
  34, // Multi button pin // PIN 34 DEAD?
  5 // Multi button light
);

Hoop hoop3(
  3, // Lane number
  48, // Clock
  49, // Data
  50, // Latch
  59, // Sensor pin
  41, // Start button pin
  6, // Start button light
  40, // Multi button pin
  9 // Multi button light
);

Hoop hoop4(
  4, // Lane number
  51, // Clock
  52, // Data
  53, // Latch
  58, // Sensor pin 
  38, // Start button pin
  7, // Start button light
  37, // Multi button pin
  8 // Multi button light
);

int numHoops = 4;
Hoop* hoops[] = {&hoop1, &hoop2, &hoop3, &hoop4};

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

  Serial.println("Ready!");
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
      case 'r':
        hoops[debugHoop]->EndGame();
        break;
      case 'x':
        hoops[debugHoop]->UpdateDisplay();
        break;
        
      case 'p':
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

