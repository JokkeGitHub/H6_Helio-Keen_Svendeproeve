#include <Braccio.h>
#include <Servo.h>

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_ver;
Servo wrist_rot;
Servo gripper;

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

int lightLevelA1 = 0;
int lightLevelA2 = 0;
int lightLevelA3 = 0;

int lightA1_Orange_Right = 0;
int lightA2_Blue_Left = 0;
int lightA3_White_UpDown = 0;

int minimumValue = 0;
int maximumValue = 180;

int horizontalRotation = 90;
int verticalRotation = 35;
String comma = ",";
String end = "\n";
String startC = "<";
String endC = ">";

void setup() {
  // Setup serial communication at baudrate 9600 for reading the light sensor
  Serial.begin(9600);
  Braccio.begin();

  moveArm(horizontalRotation, verticalRotation);
}

void loop() {
  // reads the input on analog pin A1
  lightA1_Orange_Right = analogRead(A1);
  lightA2_Blue_Left = analogRead(A2);
  lightA3_White_UpDown = analogRead(A3);

  determineLightLevels();
  determineDirection();

  compileData();
  moveArm(horizontalRotation, verticalRotation);
  delay(50);
}

void compileData() {
  Serial.print(horizontalRotation + comma + verticalRotation + comma + lightLevelA1 + end);  // Only send when moving
}

void determineLightLevels() {
  lightLevelA1 = determineLightLevel(lightA1_Orange_Right);
  lightLevelA2 = determineLightLevel(lightA2_Blue_Left);
  lightLevelA3 = determineLightLevel(lightA3_White_UpDown);
}

int determineLightLevel(int lightValue) {
  int lightLevel = 0;

  if (lightValue - llx > 0 && lightValue - llx < 100) {
    lightLevel = llx;

  } else if (lightValue - ll9 > 0 && lightValue - ll9 < 100) {
    lightLevel = ll9;

  } else if (lightValue - ll8 > 0 && lightValue - ll8 < 100) {
    lightLevel = ll8;

  } else if (lightValue - ll7 > 0 && lightValue - ll7 < 100) {
    lightLevel = ll7;

  } else if (lightValue - ll6 > 0 && lightValue - ll6 < 100) {
    lightLevel = ll6;

  } else if (lightValue - ll5 > 0 && lightValue - ll5 < 100) {
    lightLevel = ll5;

  } else if (lightValue - ll4 > 0 && lightValue - ll4 < 100) {
    lightLevel = ll4;

  } else if (lightValue - ll3 > 0 && lightValue - ll3 < 100) {
    lightLevel = ll3;

  } else if (lightValue - ll2 > 0 && lightValue - ll2 < 100) {
    lightLevel = ll2;

  } else if (lightValue - ll1 > 0 && lightValue - ll1 < 100) {
    lightLevel = ll1;

  } else {
    lightLevel = ll0;
  }

  return lightLevel;
}

void determineDirection() {
  if (lightLevelA1 < lightLevelA2 && lightLevelA1 < lightLevelA3) {
    verticalRotation = increaseValue(verticalRotation);
  } else if (lightLevelA1 > lightLevelA2 && lightLevelA1 > lightLevelA3) {
    verticalRotation = decreaseValue(verticalRotation);
  } else if (lightLevelA3 > lightLevelA2) {
    horizontalRotation = increaseValue(horizontalRotation);
  } else if (lightLevelA3 < lightLevelA2) {
    horizontalRotation = decreaseValue(horizontalRotation);
  }
}

void moveArm(int horizontalValue, int verticalValue) {
  Braccio.ServoMovement(0, horizontalValue, verticalValue, 0, 0, 0, 0);
}

int decreaseValue(int value) {
  if (value > minimumValue) {
    value -= 1;
  }
  return value;
}

int increaseValue(int value) {
  if (value < maximumValue) {
    value += 1;
  }
  return value;
}