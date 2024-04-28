#include <Braccio.h>
#include <Servo.h>

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_ver;
Servo wrist_rot;
Servo gripper;

const byte numberOfChars = 32;
char receivedChars[numberOfChars];
String receivedString;

String horizontalData;
String verticalData;
String lightValueData;

int horizontalRotation = 0;
int verticalRotation = 0;
int lightValue = 0;

boolean newData = false;
boolean handledData = false;

void setup() {
  Serial.begin(9600);
  Braccio.begin();
}

void loop() {
  listenForSignal();
  handleData();

  if (handledData == true) {
    moveArm(horizontalRotation, verticalRotation);
    handledData = false;
  }
}

void listenForSignal() {
  static boolean receiveInProgress = false;
  static byte index = 0;
  char startMarker = '<';
  char endMarker = '>';
  char receivedData;

  while (Serial.available() > 0 && newData == false) {
    receivedData = Serial.read();

    if (receiveInProgress == true) {
      if (receivedData != endMarker) {
        receivedChars[index] = receivedData;
        index++;
        if (index >= numberOfChars) {
          index = numberOfChars - 1;
        }
      } else {
        receivedChars[index] = '\0';  // terminate the string
        receiveInProgress = false;
        index = 0;
        newData = true;
      }
    }

    else if (receivedData == startMarker) {
      receiveInProgress = true;
    }
  }
}

void handleData() {
  if (newData == true) {
    newData = false;
    convertToString(receivedChars);
    splitString(receivedString);
    convertDataToIntegers();
    handledData = true;
  }
}

void convertToString(char charsToHandle[]) {
  receivedString = charsToHandle;
  splitString(receivedString);
}

void splitString(String stringToSplit) {
  int index1 = stringToSplit.indexOf(',');
  int index2 = stringToSplit.indexOf(',', index1 + 1);

  horizontalData = stringToSplit.substring(0, index1);
  verticalData = stringToSplit.substring(index1 + 1, index2);
  lightValueData = stringToSplit.substring(index2 + 1);
}

void convertDataToIntegers() {
  horizontalRotation = horizontalData.toInt();
  verticalRotation = verticalData.toInt();
  lightValue = lightValueData.toInt();
}

void moveArm(int horizontal, int vertical) {
  Braccio.ServoMovement(0, horizontal, vertical, 0, 0, 0, 0);
}