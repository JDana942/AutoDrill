#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Stepper.h>
int in_1 = 3;
int in_2 = 5;
int in_3 = 4;
int in_4 = 6;
int usr;
int bitPin = 9;
int bitSpeed;
int cutSpeed;
Stepper mystepper = Stepper(300,in_1,in_3,in_2,in_4);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600); // Debugging
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Initalizing...");
  Serial.println("Initalizing..."); // Debugging
  delay(2000);
  loop();
}

void loop() {
  Serial.println("Main Menu"); // Debugging
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1=AUTO:2=MANUAL");
  lcd.setCursor(0,1);
  lcd.print("3=TEST");
  while(true){
    usr = analogRead(A0);
    if (usr >= 128 && usr <= 130){ 
      autocontrol();
    }
    else if (usr >= 175 && usr <= 177){
      manualControl();
    }
    else if (usr >= 234 && usr <= 236){
      testsys();
    }
    else{
    }
  }
}

void autocontrol(){
  Serial.println("Auto Control"); // Debugging
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("AUTO Selected");
  delay(1000);
  setPosition();
  setbitSpeed();
  setcutSpeed();
  while(true){
    if (usr >= 611 && usr <= 613){
    }
    else if (usr >= 51 && usr <= 53){
      setcutSpeed();
    }
    else{
    }
  }
}

void manualControl(){
  Serial.println("Manual Control"); // Debugging
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Manual Selected");
  delay(1000);
  setPosition();
  setbitSpeed();
  setcutSpeed();
  while(true){
    if (usr >= 611 && usr <= 613){
    }
    else if (usr >= 51 && usr <= 53){
      setcutSpeed();
    }
    else{
    }
  }
}

void setbitSpeed(){
  Serial.println("Setting bit speed"); // Debugging
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SetBitSpeed 1-3");
  while(true){
    usr = analogRead(A0);
    if (usr >= 128 && usr <= 130){ 
     
    }
    else if (usr >= 175 && usr <= 177){
      
    }
    else if (usr >= 234 && usr <= 236){
      
    }
    else if (usr >= 42 && usr <= 44){
      break;
    }
    else if (usr >= 51 && usr <= 53){
      setPosition();
    }
    else if (usr >= 76 && usr <= 78){
      loop();
    }
    else{
  }
  return bitSpeed;
}
}

void setcutSpeed(){
  Serial.println("Setting Cut Speed"); // Debugging
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SetCutSpeed 1-5");
  while(true){
    usr = analogRead(A0);
    if (usr >= 89 && usr <= 91){ 
    }
    else if (usr >= 108 && usr <= 110){
    }
    else if (usr >= 42 && usr <= 44){
      break;
    }
    else if (usr >= 51 && usr <= 53){
      setbitSpeed();
    }
    else if (usr >= 76 && usr <= 78){
      loop();
    }
    else{
  }
  return cutSpeed;
}
}

void setPosition(){
  Serial.println("Setting Position"); // Debugging
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Course ADJ");
  lcd.setCursor(0,1);
  lcd.print("Finished?");
  mystepper.setSpeed(25);
  while(true){
    usr = analogRead(A0);
    if (usr >= 89 && usr <= 91){ 
      mystepper.step(5);
    }
    else if (usr >= 108 && usr <= 110){
      mystepper.step(-5);
    }
    else if (usr >= 42 && usr <= 44){
      break;
    }
    else if (usr >= 76 && usr <= 78){
      loop();
    }
    else{
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Fine ADJ");
  lcd.setCursor(0,1);
  lcd.print("Finished?");
  mystepper.setSpeed(5);
  while(true){
    usr = analogRead(A0);
    if (usr >= 89 && usr <= 91){ 
      mystepper.step(3);
    }
    else if (usr >= 108 && usr <= 110){
      mystepper.step(-3);
    }
    else if (usr >= 42 && usr <= 44){
      break;
    }
    else if (usr >= 51 && usr <= 53){
      setPosition();
    }
    else if (usr >= 76 && usr <= 78){
      loop();
    }
    else{
  }
  return;
}
}

void testsys(){
   Serial.println("Testing SYS"); // Debugging
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("System Test");
   lcd.setCursor(0,1);
   lcd.print("Ready?");
   while(true){
    usr = analogRead(A0);
    if (usr >= 611 && usr <= 613){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("System Testing");
      lcd.setCursor(0,1);
      lcd.print("KEEP CLEAR!");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Testing Drill");
      analogWrite(bitPin, 100);
      delay(1000);
      analogWrite(bitPin, 150);
      delay(1000);
      analogWrite(bitPin, 200);
      delay(1000);
      digitalWrite(bitPin, 1);
      delay(1000);
      digitalWrite(bitPin, 0);
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Testing Stepper");
      mystepper.setSpeed(5);
      mystepper.step(200);
      delay(3000);
      mystepper.step(-200);
      delay(3000);
      mystepper.setSpeed(25);
      mystepper.step(200);
      delay(3000);
      mystepper.step(-200);
      delay(3000);
      digitalWrite(in_1,0);
      digitalWrite(in_2,0);
      digitalWrite(in_3,0);
      digitalWrite(in_4,0);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Test Complete");
      break;
    }
    else if (usr >= 51 && usr <= 53){
      break;
    }
    else if (usr >= 76 && usr <= 78){
      loop();
    }
  }
  return;
}
