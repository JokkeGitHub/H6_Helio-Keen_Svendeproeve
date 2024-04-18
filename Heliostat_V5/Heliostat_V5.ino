#include <Braccio.h>
#include <Servo.h>

#pragma region Datatypes
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_ver;
Servo wrist_rot;
Servo gripper;

int nd = 50;  // No Delay
int nr = 90;  // No Rotation
int ng = 70;  // No Grip

int hr = 150;   // Horizontal Rotation
int hrm = 30;   // Horizontal Rotation Maximum
int hrx = 150;  // Horizontal Rotation Minimum
int vr = 175;   // Vertical Rotation
int vrm = 125;  // Vertical Rotation Maximum
int vrx = 175;  // Vertical Rotation Minimum

bool increaseHorizontalValue = true;
bool increaseVerticalValue = true;
bool moveVertically = false;
int verticalTicks = 0;
bool resetPosition = false;

// reads the input on analog pin A0
int lightValue = 0;
int lightLevel = 0;

int savedLightLevel = 0;
int savedHr = 0;
int savedVr = 0;

int tempLightValue = 0;
int tempHr = 0;
int tempVr = 0;

int ll0 = 0;     // Light level 0
int ll1 = 100;   // Light level 1
int ll2 = 200;   // Light level 2
int ll3 = 300;   // Light level 3
int ll4 = 400;   // Light level 4
int ll5 = 500;   // Light level 5
int ll6 = 600;   // Light level 6
int ll7 = 700;   // Light level 7
int ll8 = 800;   // Light level 8
int ll9 = 900;   // Light level 9
int llx = 1000;  // Light level 10

bool isDaytime = true;
bool routeReceived = false;
#pragma endregion

void setup() {
  Serial.begin(9600);
  Braccio.begin();

  moveArm(hr, vr);
  lightLevel = llx;
}

void loop() {
  
  // Check for inputs

  if (isDaytime == false) {
    // Await input
  } else {
    if (routeReceived == true) {
      // get route
    } else {
      getLightValue();
      maintainLightLevel();
    }
  }
}

#pragma region Alter Data
bool toggleBoolean(bool boolean) {
  if (boolean == true) {
    boolean = false;
  } else {
    boolean = true;
  }

  return boolean;
}

int resetValue(int value) {
  value = 0;
  return value;
}

int decreaseValue(int value) {
  value -= 1;
  return value;
}

int increaseValue(int value) {
  value += 1;
  return value;
}

int increaseValueBy10(int value) {
  value += 10;
  return value;
}

#pragma endregion

#pragma region Saving Data
void saveLightLevel() {
  savedLightLevel = lightLevel;

  Serial.print("Saved Light Level: ");
  Serial.print(savedLightLevel);
}

void saveRotation() {
  savedHr = hr;
  savedVr = vr;

  String hrString = " HR, ";
  String vrString = " VR";
  Serial.print(" at positions: ");
  Serial.println(savedHr + hrString + savedVr + vrString);
}
#pragma endregion

#pragma region Temporary Data

void saveTempRotation() {
  tempHr = hr;
  tempVr = vr;
}

void resetTempData() {
  tempLightValue = resetValue(tempLightValue);
  tempHr = resetValue(tempHr);
  tempVr = resetValue(tempVr);
}
#pragma endregion

#pragma region Light Value& Light Levels
void getLightValue() {
  // reads the input on analog pin A5
  lightValue = analogRead(A5);
}

void printLightValue() {
  // Print out the values to read in the Serial Monitor
  Serial.print("Analog reading (0-1023): ");
  Serial.println(lightValue);
}

int determineLightLevel(int lightLevelValue) {

  getLightValue();
  printLightValue();

  if (lightValue - llx > 0 && lightValue - llx < 100) {
    lightLevelValue = llx;
    Serial.println("Light Level is 10");
  } else if (lightValue - ll9 > 0 && lightValue - ll9 < 100) {
    lightLevelValue = ll9;
    Serial.println("Light Level is 9");
  } else if (lightValue - ll8 > 0 && lightValue - ll8 < 100) {
    lightLevelValue = ll8;
    Serial.println("Light Level is 8");
  } else if (lightValue - ll7 > 0 && lightValue - ll7 < 100) {
    lightLevelValue = ll7;
    Serial.println("Light Level is 7");
  } else if (lightValue - ll6 > 0 && lightValue - ll6 < 100) {
    lightLevelValue = ll6;
    Serial.println("Light Level is 6");
  } else if (lightValue - ll5 > 0 && lightValue - ll5 < 100) {
    lightLevelValue = ll5;
    Serial.println("Light Level is 5");
  } else if (lightValue - ll4 > 0 && lightValue - ll4 < 100) {
    lightLevelValue = ll4;
    Serial.println("Light Level is 4");
  } else if (lightValue - ll3 > 0 && lightValue - ll3 < 100) {
    lightLevelValue = ll3;
    Serial.println("Light Level is 3");
  } else if (lightValue - ll2 > 0 && lightValue - ll2 < 100) {
    lightLevelValue = ll2;
    Serial.println("Light Level is 2");
  } else if (lightValue - ll1 > 0 && lightValue - ll1 < 100) {
    lightLevelValue = ll1;
    Serial.println("Light Level is 1");
  } else {
    lightLevelValue = ll0;
    Serial.println("Light Level is 0");
  }

  return lightLevelValue;
}

void maintainLightLevel() {

  if (lightValue > tempLightValue) {
    tempLightValue = lightValue;
    tempHr = hr;
    tempVr = vr;
  }

  if (hr == hrm && vr == vrm && lightValue - lightLevel < 0) {
    if (resetPosition == true) {
      moveToStartPosition();
      lightLevel = llx;
    } else {
      Serial.println("WE MADE IT IN HERE !!!!!!!!!!!!!!!");
      hr = tempHr;
      vr = tempVr;

      moveArm(hr, vr);
      lightLevel = determineLightLevel(lightLevel);
      saveLightLevel();
      saveRotation();

      // Submit to backend?
    }

    resetTempData();
    resetPosition = toggleBoolean(resetPosition);
  } else if (lightValue - lightLevel >= 100) {

    lightLevel = determineLightLevel(lightLevel);
    saveLightLevel();
    saveRotation();

    // Submit to backend?
  } else if (lightValue - lightLevel < 0) {

    if (moveVertically == true) {
      movePositionVertical();
    } else {
      movePositionHorizontal();
    }
  }
}
#pragma endregion

#pragma region Automated Base Movement
void moveArm(int horizontalValue, int verticalValue) {
  Braccio.ServoMovement(nd, horizontalValue, nr, verticalValue, nr, nr, ng);
}

void moveToStartPosition() {
  hr = hrx;
  vr = vrx;
  moveArm(hr, vr);
}

void movePositionHorizontal() {

  if (increaseHorizontalValue == true && hr < hrx) {

    hr = increaseValue(hr);

  } else if (increaseHorizontalValue == true && hr == hrx) {

    increaseHorizontalValue = toggleBoolean(increaseHorizontalValue);
    moveVertically = toggleBoolean(moveVertically);
    verticalTicks = increaseValueBy10(verticalTicks);

  } else if (increaseHorizontalValue == false && hr > hrm) {

    hr = decreaseValue(hr);

  } else if (increaseHorizontalValue == false && hr == hrm) {

    increaseHorizontalValue = toggleBoolean(increaseHorizontalValue);
    moveVertically = toggleBoolean(moveVertically);
    verticalTicks = increaseValueBy10(verticalTicks);
  }

  moveArm(hr, vr);
}

void movePositionVertical() {

  if (verticalTicks != 0) {

    if (increaseVerticalValue == true && vr < vrx) {
      vr = increaseValue(vr);
      verticalTicks = decreaseValue(verticalTicks);
    } else if (increaseVerticalValue == true && vr == vrx) {
      increaseVerticalValue = toggleBoolean(increaseVerticalValue);
      moveVertically = toggleBoolean(moveVertically);
    } else if (increaseVerticalValue == false && vr > vrm) {
      vr = decreaseValue(vr);
      verticalTicks = decreaseValue(verticalTicks);
    } else if (increaseVerticalValue == false && vr == vrm) {
      increaseVerticalValue = toggleBoolean(increaseVerticalValue);
      moveVertically = toggleBoolean(moveVertically);
    }

    moveArm(hr, vr);
  } else {
    moveVertically = toggleBoolean(moveVertically);
  }
}
#pragma endregion
