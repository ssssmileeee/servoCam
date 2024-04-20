#include <ServoSmooth.h>                //https://alexgyver.ru/servosmooth/


uint8_t reverseSignalPin = A0;          //set your reverse lights analog input pin number here
uint8_t servoPin = 2;                   //set your servo data pin number here

uint8_t maxServoAngle = 127;            //absolute maximum angle that the servo supports
uint8_t closedAngle = 127;              //servo position angle in closed camera state (need adjust in your setup)
uint8_t openedAngle = 20;               //servo position angle in opened camera state (need adjust in your setup)
int32_t reverseSignalMinVoltage = 450;  //minimum ananog value to be detected as reard ligts enabled (need adjust in your setup)
int32_t openCamDelayMs = 500;           //signal delay in ms, below which servo will not be triggered

uint32_t stateUpdatedTime = 0;
bool camEnabled = false;
bool lastCommandExecuted = true;
ServoSmooth servo(maxServoAngle);



void setup() {
  servo.attach(servoPin, closedAngle);
  servo.smoothStart();
  servo.setSpeed(500);
  servo.setAccel(1.2);
}

void loop() {
  servo.tick();

  int32_t currentVoltageAnalogValue = analogRead(reverseSignalPin);

  if (currentVoltageAnalogValue >= reverseSignalMinVoltage && camEnabled == false) {
    camEnabled = true;
    stateUpdatedTime = millis();
    lastCommandExecuted = false;
  }

  if (currentVoltageAnalogValue < reverseSignalMinVoltage && camEnabled == true) {
    camEnabled = false;
    stateUpdatedTime = millis();
    lastCommandExecuted = false;
  }

  if (lastCommandExecuted == false) {
    if (camEnabled == false) {
      closeCam();
    } else if (camEnabled == true && (millis() - stateUpdatedTime) > openCamDelayMs) {
      openCam();
    }
  }
}

void openCam() {
  servo.setTargetDeg(openedAngle);  
  lastCommandExecuted = true;
}

void closeCam() {
  servo.setTargetDeg(closedAngle);
  lastCommandExecuted = true;
}
