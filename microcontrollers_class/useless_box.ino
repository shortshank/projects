/*
Useless box. This project was for the purpose of working with servos.
Ended up being incredibly difficult to work with, as tolerances were slightly off,
and things just kept falling apart. Worked decently well in the end.
*/

#include <Servo.h>

const int switchPins[] = {2, 3, 4};  //switches

const int servoArmPin  = 9;   //servo that flips the switch
const int servoBasePin = 10;  //servo that positions the arm

//servo setup
Servo servoArm;
Servo servoBase;

//servo angles for each switch
int baseAngles[] = {50, 80, 120};

void setup() {
  //setup pin inputs and previous switch states
  for (int i = 0; i < 3; i++) {
    pinMode(switchPins[i], INPUT_PULLUP);
  }

  //setup servos
  servoArm.attach(servoArmPin);
  servoBase.attach(servoBasePin);

  servoArm.write(0);
  servoBase.write(baseAngles[1]); //center switch

  randomSeed(analogRead(A0)); //randomize seed
  Serial.begin(9600);
  Serial.println("ready");
}

void loop() {
  //check each switch
  for (int i = 0; i < 3; i++) {
    if (digitalRead(switchPins[i]) == LOW) {
      //Serial.print(i);
      Serial.print(digitalRead(i));
      hitSwitch(switchPins[i], i); //flip switch
    }
  }
}

void hitSwitch(int switchPin, int index) {
  moveServoSmooth(servoBase, servoBase.read(), baseAngles[index], 8);
  moveServoSmooth(servoArm, 0, 155, 5);
  while(digitalRead(switchPin) == LOW) {
    delay(600);
  }
  moveServoSmooth(servoArm, 155, 0, 5);
  delay(400);
  moveServoSmooth(servoBase, servoBase.read(), baseAngles[1], 8);
}

//helper method to move the switch from one angle to another smoothly
void moveServoSmooth(Servo &servo, int fromAngle, int toAngle, int stepDelay) {
  if (fromAngle < toAngle) {
    for (int pos = fromAngle; pos <= toAngle; pos++) {
      servo.write(pos);
      delay(stepDelay);
    }
  } else {
    for (int pos = fromAngle; pos >= toAngle; pos--) {
      servo.write(pos);
      delay(stepDelay);
    }
  }
}
