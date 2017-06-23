class SevenSeg
{
  int numDigits;
    int firstPin;
    int numSegments = 8;
    
    int ON = 4095;
    int OFF = 0;
 
    // Set segment pins
    int _A=6;
    int _B=7;
    int _C=2;
    int _D=1;
    int _E=0;
    int _F=5;
    int _G=4;
    int _DP=-1; // DP initially not assigned

  unsigned long previousMillis;
 
  public:
  int numPins;
  int *pins;

  SevenSeg()
  {
  previousMillis = 0;
  }
 
  void Setup(int nd)
  {
  numDigits = nd;
    numPins = numDigits * numSegments;
    //int *pins = new int[numPins]();

    // https://arduino.stackexchange.com/questions/3774/how-can-i-declare-an-array-of-variable-size-globally
  // if (pins != 0) {
  //  pins = (int*) realloc(pins, numPins * sizeof(int));
  // } else {
  //  pins = (int*) malloc(numPins * sizeof(int));
  // }
    // Allocate array of pin values they way Adafruit_TLC5947 does
    pins = (int *)malloc(numPins * sizeof(int));
    memset(pins, 0, numPins * sizeof(int));

    //Serial.println((String)"Pins size: " + sizeof(pins));
    Clear();
  }

  void PrintStatus()
  {
    for(int i=0; i<numPins; i++) {
      Serial.print((String)"Pin " + i + ": " + pins[i] + ", ");
    }
    Serial.println();
  }

  int* GetAllPins()
  {
   return pins;    
  }

  void Clear()
  {
    for(int i=0; i<numPins; i++) {
      pins[i] = OFF;
    }
  }

  void SetPin(int pin, int val)
  {
    pins[pin] = val;
  }

  int GetPin(int pin)
  {
    int val = pins[pin];
    Serial.println((String)"Pin " + pin + ": " + val);
  }

  void Set(int num){
    for(int i=numDigits-1;i>=0;i--){
        int nextPiece = num % 10L;
        if(num || i==numDigits-1){
            SetDigit(i, nextPiece);
        } else {
            // Fill digits with zero
            SetDigit(i, 0);
        }
        num /= 10;
    }
  }

  void SetDigit(int digit, int num){
  
    if (digit > numDigits) {
        
        // We don't have enough digits. Cut this number off
        
    } else {
    
        Serial.println((String)"    Setting digit '" + digit + "' to: " + num);
  
        // Turn off all LEDs
        SetSeg(digit, _A, OFF);
        SetSeg(digit, _B, OFF);
        SetSeg(digit, _C, OFF);
        SetSeg(digit, _D, OFF);
        SetSeg(digit, _E, OFF);
        SetSeg(digit, _F, OFF);
        SetSeg(digit, _G, OFF);
        
        if(num==1){
          SetSeg(digit, _B, ON);
          SetSeg(digit, _C, ON);
        }                     
                              
        else if(num==2){       
          SetSeg(digit, _A, ON);
          SetSeg(digit, _B, ON);
          SetSeg(digit, _G, ON);
          SetSeg(digit, _E, ON);
          SetSeg(digit, _D, ON);
        }                     
                              
        else if(num==3){       
          SetSeg(digit, _A, ON);
          SetSeg(digit, _B, ON);
          SetSeg(digit, _G, ON);
          SetSeg(digit, _C, ON);
          SetSeg(digit, _D, ON);
        }                     
                              
        else if(num==4){       
          SetSeg(digit, _F, ON);
          SetSeg(digit, _G, ON);
          SetSeg(digit, _B, ON);
          SetSeg(digit, _C, ON);
        }                     
                              
        else if(num==5){       
          SetSeg(digit, _A, ON);
          SetSeg(digit, _F, ON);
          SetSeg(digit, _G, ON);
          SetSeg(digit, _C, ON);
          SetSeg(digit, _D, ON);
        }                     
                              
        else if(num==6){       
          SetSeg(digit, _A, ON);
          SetSeg(digit, _F, ON);
          SetSeg(digit, _E, ON);
          SetSeg(digit, _D, ON);
          SetSeg(digit, _C, ON);
          SetSeg(digit, _G, ON);
        }                     
                              
        else if(num==7){       
          SetSeg(digit, _A, ON);
          SetSeg(digit, _B, ON);
          SetSeg(digit, _C, ON);
        }                     
                              
        else if(num==8){       
          SetSeg(digit, _A, ON);
          SetSeg(digit, _B, ON);
          SetSeg(digit, _C, ON);
          SetSeg(digit, _D, ON);
          SetSeg(digit, _E, ON);
          SetSeg(digit, _F, ON);
          SetSeg(digit, _G, ON);
        }                     
                              
        else if(num==9){       
          SetSeg(digit, _G, ON);
          SetSeg(digit, _F, ON);
          SetSeg(digit, _A, ON);
          SetSeg(digit, _B, ON);
          SetSeg(digit, _C, ON);
          SetSeg(digit, _D, ON);
        }                     
                              
        else if(num==0){       
          SetSeg(digit, _A, ON);
          SetSeg(digit, _B, ON);
          SetSeg(digit, _C, ON);
          SetSeg(digit, _D, ON);
          SetSeg(digit, _E, ON);
          SetSeg(digit, _F, ON);
        }
  
        else {
          Serial.print("Unknown number or bad type to print: ");
          Serial.println(num);
        }
  
    }
        
  }

  void SetSeg(int digit, int seg, int pwm_val) {
        //Serial.print("        Setting pin '");
        //Serial.print(pinNum(digit, seg));
        //Serial.print("' to: ");
        //Serial.println(pwm_val);
  
        //tlc.setPWM(pinNum(digit, seg), pwm_val);
        int pin = pinNum(digit, seg);
        pins[pin] = pwm_val;
  }
  
  int pinNum(int digit, int seg) {
      return seg + (digit * numSegments);
  }

};
