#include <Braccio.h>
#include <Servo.h>

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_ver;
Servo wrist_rot;
Servo gripper;

int nd = 0; // No Delay
int nr = 90; // No Rotation
int ng = 73; // No Grip

int hr = 180; // Horizontal Rotation
int hrm = 0; // Horizontal Rotation Minimum
int hrx = 180; // Horizontal Rotation Maximum
int vr = 140; // Vertical Rotation
int vrm = 140; // Vertical Rotation Minimum
int vrx = 180; // Vertical Rotation Maximum

bool increaseHorizontalValue = true;
bool increaseVerticalValue = true;
bool moveVertically = false;
int verticalTicks = 0;

// reads the input on analog pin A0
int lightValue = 0;
int lightLevel = 0;

int savedLightLevel = 0;
int savedHr = 0;
int savedVr = 0;

int tempLightLevel = 0;
int tempHr = 0;
int tempVr = 0;

int ll0 = 0; // Light level 0
int ll1 = 100; // Light level 1
int ll2 = 200; // Light level 2
int ll3 = 300; // Light level 3
int ll4 = 400; // Light level 4
int ll5 = 500; // Light level 5
int ll6 = 600; // Light level 6
int ll7 = 700; // Light level 7
int ll8 = 800; // Light level 8
int ll9 = 900; // Light level 9
int llx = 1000; // Light level 10

void setup() {
  Serial.begin(9600);
  Braccio.begin();

  determineLightLevel();
  saveLightLevel();
}

void loop() {
  // Given inputs true/false, automate or not
  
  getLightValue();
  printLightValue();
  maintainLightLevel();

  //delay(1000);
}

void getLightValue(){
  // reads the input on analog pin A0
  lightValue = analogRead(A7);
}

void printLightValue(){
  // Print out the values to read in the Serial Monitor
  Serial.print("Analog reading (0-1023): ");
  Serial.println(lightValue);
}

void saveLightLevel(){
  savedLightLevel = lightLevel;
}

void saveRotation(){
  savedHr = hr;
  savedVr = vr;
}

void saveTempLightLevel(){
  tempLightLevel = lightLevel;
}

void saveTempRotation(){
  tempHr = hr;
  tempVr = vr;
}

void determineLightLevel(){
  
  getLightValue();
  printLightValue();

  if(lightValue - llx > 0 && lightValue - llx < 100){
    lightLevel = llx;
    Serial.println("Light Level is 10");
  }
  else if(lightValue - ll9 > 0 && lightValue - ll9 < 100){
    lightLevel = ll9;
    Serial.println("Light Level is 9");
  }
  else if(lightValue - ll8 > 0 && lightValue - ll8 < 100){
    lightLevel = ll8;
    Serial.println("Light Level is 8");
  }
  else if(lightValue - ll7 > 0 && lightValue - ll7 < 100){
    lightLevel = ll7;
    Serial.println("Light Level is 7");
  }
  else if(lightValue - ll6 > 0 && lightValue - ll6 < 100){
    lightLevel = ll6;
    Serial.println("Light Level is 6");
  }
  else if(lightValue - ll5 > 0 && lightValue - ll5 < 100){
    lightLevel = ll5;
    Serial.println("Light Level is 5");
  }
  else if(lightValue - ll4 > 0 && lightValue - ll4 < 100){
    lightLevel = ll4;
    Serial.println("Light Level is 4");
  }
  else if(lightValue - ll3 > 0 && lightValue - ll3 < 100){
    lightLevel = ll3;
    Serial.println("Light Level is 3");
  }
  else if(lightValue - ll2 > 0 && lightValue - ll2 < 100){
    lightLevel = ll2;
    Serial.println("Light Level is 2");
  }
  else if(lightValue - ll1 > 0 && lightValue - ll1 < 100){
    lightLevel = ll1;
    Serial.println("Light Level is 1");
  }
  else{
    lightLevel = ll0;
    Serial.println("Light Level is 0");
  }
}

void maintainLightLevel(){
  if(lightValue - lightLevel >= 100){
    determineLightLevel();
    saveLightLevel();
    saveRotation();
  }
  else if(lightValue - lightLevel > 0 && lightValue - lightLevel < 100){
    // Stay
  }
  else if(lightValue - lightLevel < 0)
  {    
    // Find new position
    if(moveVertically == true){
      movePositionVertical();
    }
    else{
      movePositionHorizontal();
    }
    //determineLightLevel();
  }
}

void movePositionHorizontal(){

  if(increaseHorizontalValue == true && hr < hrx){
    hr += 1;
  }
  else if(increaseHorizontalValue == true && hr == hrx){
    giveVerticalTicks();
    toggleMovementDirection();
    toggleHorizontalDirection();
  }
  else if(increaseHorizontalValue == false && hr > hrm){
    hr -= 1;
  }
  else if(increaseHorizontalValue == false && hr == hrm){
    toggleHorizontalDirection();
    giveVerticalTicks();
    toggleMovementDirection();
  }

  Braccio.ServoMovement(nd, hr, nr, vr, nr, nr, ng);
}

void movePositionVertical(){

  if(verticalTicks != 0){

    if(increaseVerticalValue == true && vr < vrx){
      vr += 1;
      decreaseVerticalTicks();
    }
    else if(increaseVerticalValue == true && vr == vrx){
      toggleVerticalDirection();
      toggleMovementDirection();
    }
    else if(increaseVerticalValue == false && vr > vrm){
      vr -= 1;
      decreaseVerticalTicks();
    }
    else if(increaseVerticalValue == false && vr == vrm){
      toggleVerticalDirection();
      toggleMovementDirection();
    }

    Braccio.ServoMovement(nd, hr, nr, vr, nr, nr, ng);
  }
  else{
    toggleMovementDirection();
  }
}

void toggleMovementDirection(){
  if(moveVertically == true){
    moveVertically = false;
  }
  else{
    moveVertically = true;
  }
}

void toggleHorizontalDirection(){
  if(increaseHorizontalValue == true){
    increaseHorizontalValue = false;
  }
  else{
    increaseHorizontalValue = true;
  }  
}

void toggleVerticalDirection(){
  if(increaseVerticalValue == true){
    increaseVerticalValue = false;
  }
  else{
    increaseVerticalValue = true;
  }    
}

void giveVerticalTicks(){
  verticalTicks = 10;
}

void decreaseVerticalTicks(){
  verticalTicks -= 1;
}