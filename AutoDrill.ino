//Version 01.02
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

int DIRPin = 12;
int STEPPin = 11;
int bitPin = 9;
int i, cutSpeed, stepDelay, stepNum, usr, cutTemp, DIR, resumeCut;
LiquidCrystal_I2C lcd(0x27, 16, 2);
const byte ROWS = 4; 
const byte COLS = 4;
char customKey, holdKey; 
char depthBuffer[5];
long cutDepth;
char *p;

char hexaKeys[ROWS][COLS] = {
  {'B', 'A', 'D', 'U'},
  {'E', '1', '4', '7'},
  {'0', '2', '5', '8'},
  {'X', '3', '6', '9'}
};

byte rowPins[ROWS] = {6, 7, 8, 10}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
void setup() {
  pinMode(DIRPin, OUTPUT);
  pinMode(STEPPin, OUTPUT);
  pinMode(bitPin, OUTPUT);
  reset();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Initalizing...");
  delay(2000);
  loop();
}

void loop() {
  reset();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("A = AUTO:");
  lcd.setCursor(0,1);
  lcd.print("B = MANUAL:");
  while(true){
    customKey = customKeypad.getKey();
    if (customKey == 'A'){ 
      autocontrol();
      break;
    }
    else if (customKey == 'B'){
      manualControl();
      break;
    }
    else{}
  }
}

void autocontrol(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Auto Selected");
  delay(1000);
  setPosition();
  setcutSpeed();
  setcutDepth();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("E When Ready");
  while(true){
    customKey = customKeypad.getKey();
    if (customKey == 'E'){
      _step(1,10,resumeCut);
      delay(10*resumeCut);
      analogWrite(bitPin, 30);
      delay(500);
      _step(1,(100/cutSpeed),cutDepth);
      delay((10*cutDepth)/cutSpeed);
      _step(0,10,cutDepth+50);
      delay(10*cutDepth);
      digitalWrite(bitPin, 0);
      resumeCut = 50;
        
    }
    else if (customKey == 'X'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("EXITING...");
      delay(1000);
      loop();
    }
    else{}
  }
  loop();
}

void manualControl(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MANUAL SELECTED");
  delay(1000);
  setPosition();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("E When Ready");
  delay(500);
  while(true){
    customKey = customKeypad.getKey();
    if (customKey == 'E'){
      analogWrite(bitPin, 30);
      cutSpeed = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("CUTSPEED = 1");
      lcd.setCursor(0,1);
      lcd.print("X = EXIT");
      holdKey = 'Z';
      delay(500);
      while(true){
        customKey = customKeypad.getKey();
        if (customKey){
          holdKey = customKey;
        }
        else if(customKeypad.getState() == HOLD){
          if (holdKey == 'U'){
            _step(0, (100/cutSpeed), 1);
          }
          else if (holdKey == 'D'){
          _step(1, (100/cutSpeed), 1);
          }
          else{}
        }
        else if (holdKey == 'A' && cutSpeed < 8){
          holdKey = 'Z';
          cutSpeed++;
          lcd.setCursor(11,0);
          lcd.print(cutSpeed);
          delay(100);
        }
        else if (holdKey == 'B' && cutSpeed > 1){
          holdKey = 'Z';
          cutSpeed--;
          lcd.setCursor(11,0);
          lcd.print(cutSpeed);
          delay(100);
        }
        else if (holdKey == 'X'){
          digitalWrite(bitPin, 0);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("EXITING...");
          delay(1000);
          loop();
        }
        else{}
      }
    }
    else if (customKey == 'X'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("EXITING...");
      delay(1000);
      loop();
    }
    else{}
  }
  return;
}

void setPosition(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("U/D Course ADJ");
  lcd.setCursor(0,1);
  lcd.print("A/B Fine ADJ");
  delay(500);
  while(true){
    customKey = customKeypad.getKey();
    if (customKey){
      holdKey = customKey;
    }
    else if(customKeypad.getState() == HOLD){
      if (holdKey == 'U'){
        _step(0, 5, 1);
      }
      else if (holdKey == 'D'){
       _step(1, 5, 1);
      }
      else if (holdKey == 'A'){
        _step(0, 50, 1);
      }
      else if (holdKey == 'B'){
        _step(1, 50, 1);
      }
      else{}
    }
    else if (holdKey == 'E'){
      break;
    }
    else if (holdKey == 'X'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("EXITING...");
      delay(1000);
      loop();
    }
    else{}
  }
  return;
}

int setcutSpeed(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SetCutSpeed 1-9");
  lcd.setCursor(0,1);
  lcd.print("CUTSPEED = ");
  delay(500);
  while(true){
    customKey = customKeypad.getKey();
    if(isDigit(customKey) == true && customKey!= '0'){
      cutSpeed = customKey - 48;
      lcd.setCursor(11,1);
      lcd.print(cutSpeed);
    }
    else if (customKey == 'E'){
      break;
    }
    else if (customKey == 'X'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("EXITING...");
      delay(1000);
      loop();
    }
    else{}
  }
  return cutSpeed;
}

int setcutDepth(){
  i = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("DEPTH*(1/10mm)");
  delay(500);
  while(true){
    customKey = customKeypad.getKey();
    if(isDigit(customKey) == true && i < 4){
      depthBuffer[i] = customKey;
      lcd.setCursor(i,1);
      lcd.print(depthBuffer[i]);
      i++;
    }
    else if(customKey == 'B' && i > 0){
      i--;
      lcd.setCursor(i,1);
      depthBuffer[i] = '\0';
      lcd.print(" ");
    }
    else if(customKey == 'E'){
      cutDepth = strtol(depthBuffer, *p, 10);
      break;
    }
    else if(customKey == 'X'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("EXITING...");
      delay(1000);
      loop();
    }
    else{}
  }
  cutDepth = cutDepth/5;
  return cutDepth;
}

void _step(int DIR, int stepDelay, int stepNum){ // 0 is up and 1 is down
  digitalWrite(DIRPin, DIR);
  for (i = 0; i < stepNum; i++){
    digitalWrite(STEPPin, 1);
    delay(stepDelay);
    digitalWrite(STEPPin, 0); 
    delay(stepDelay); 
  } 
  return;
}

void reset(){
  holdKey = '\0';
  for(i = 0; i < sizeof(depthBuffer); i++){
    depthBuffer[i] = '\0';}
  i = 0;
  resumeCut = 0;
  digitalWrite(12, 0);
  digitalWrite(11, 0);
  digitalWrite(9, 0);
  return;
}
