#include <Braccio.h>
#include <Servo.h>

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_ver;
Servo wrist_rot;
Servo gripper;

const byte numChars = 32;
char receivedChars[numChars];
String receivedString;

int horizontalRotation = 0;
int verticalRotation = 0;
int lightValue = 0;

boolean newData = false;

void setup() {
    Serial.begin(9600);
    Braccio.begin();
    Serial.println("<Arduino is ready>");
}

void loop() {
    recvWithStartEndMarkers();
    showNewData();
    //Serial.println(receivedChars[0]);
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        Serial.println(receivedChars);

        convertToString(receivedChars);

        newData = false;
    }
}

void convertToString(char received[]){
  receivedString = receivedChars;
  splitDivideString(receivedString);
}

void splitDivideString(String string){
  String substring1;
  String substring2;
  String substring3;
  
  int i1 = string.indexOf(',');
  int i2 = string.indexOf(',', i1+1);
  
  String firstValue = string.substring(0, i1);
  Serial.println(firstValue);
  String secondValue = string.substring(i1 + 1, i2);
  Serial.println(secondValue);
  String thirdValue = string.substring(i2 + 1);
  Serial.println(thirdValue);

  horizontalRotation = firstValue.toInt();
  Serial.println(horizontalRotation);
  verticalRotation = secondValue.toInt();
  Serial.println(verticalRotation);
  lightValue = thirdValue.toInt();
  Serial.println(lightValue);
  
  moveArm(horizontalRotation, verticalRotation);
}

void moveArm(int horizontal, int vertical){
  Serial.println("Made it here?");
  Braccio.ServoMovement(0, horizontal, vertical, 0, 0, 0, 0);
}