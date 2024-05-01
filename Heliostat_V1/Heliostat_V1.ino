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

// reads the input on analog pin A0
int lightValue = 0;

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
}

void loop() {

  // reads the input on analog pin A0
  lightValue = analogRead(A7);

  // Print out the values to read in the Serial Monitor
  Serial.print("Analog reading (0-1023): ");
  Serial.print(lightValue);

  // Use the value to determine how dark it is 
  // (Try tweaking these to make it more accurate)
  if (lightValue < 10) 
  {
    Serial.println(" - Dark");
  } 
  else if (lightValue < 200) 
  {
    Serial.println(" - Dim");
  } 
  else if (lightValue < 500) 
  {
    Serial.println(" - Light");
  } 
  else if (lightValue < 800) 
  {
    Serial.println(" - Bright");
  } 
  else 
  {
    Serial.println(" - Very bright");
  }
  // 0-180, 15-165, 150-180
  // Base, Shoulder, Elbow
  ///* 
  Braccio.ServoMovement(nd, 90, 35, 180, nr, nr, ng);
  Braccio.ServoMovement(nd, 45, 35, 150, nr, nr, ng);
  Braccio.ServoMovement(nd, 0, 35, 150, nr, nr, ng);
  Braccio.ServoMovement(nd, 45, 35, 180, nr, nr, ng);
  Braccio.ServoMovement(nd, 90, 35, 180, nr, nr, ng);
  Braccio.ServoMovement(nd, 125, 35, 150, nr, nr, ng);
  Braccio.ServoMovement(nd, 180, 35, 180, nr, nr, ng);
 //*/
}

/*
void loop() {

  // reads the input on analog pin A0
  lightValue = analogRead(A7);

  // Print out the values to read in the Serial Monitor
  Serial.print("Analog reading (0-1023): ");
  Serial.print(lightValue);

  // Use the value to determine how dark it is 
  // (Try tweaking these to make it more accurate)
  if (lightValue < 10) 
  {
    Serial.println(" - Dark");
  } 
  else if (lightValue < 200) 
  {
    Serial.println(" - Dim");
  } 
  else if (lightValue < 500) 
  {
    Serial.println(" - Light");
  } 
  else if (lightValue < 800) 
  {
    Serial.println(" - Bright");
  } 
  else 
  {
    Serial.println(" - Very bright");
  }
  // 0-180, 15-165, 150-180
  // Base, Shoulder, Elbow
  ///* 
  Braccio.ServoMovement(nd, 90, 35, 180, nr, nr, ng);
  Braccio.ServoMovement(nd, 45, 35, 150, nr, nr, ng);
  Braccio.ServoMovement(nd, 0, 35, 150, nr, nr, ng);
  Braccio.ServoMovement(nd, 45, 35, 180, nr, nr, ng);
  Braccio.ServoMovement(nd, 90, 35, 180, nr, nr, ng);
  Braccio.ServoMovement(nd, 125, 35, 150, nr, nr, ng);
  Braccio.ServoMovement(nd, 180, 35, 180, nr, nr, ng);
 //
}//*/
