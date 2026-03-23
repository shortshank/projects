/*
Small project that used a photoresistor to estimate the time of day using the light level at my windowsill
*/

const int photoPin = A0; //pin that measures photoresistor

int photoValue = 0; //value of photoresistor

int photoMapValue = 0; // mapped photoresistor value

const int timeIndex[] = {924, 995, 1010, 1012}; //index of values of measured brightness from 8:00 AM to 11:00 AM, ordered from 8:00 AM (lowest) to 11:00 AM (highest)

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the value from the various analog sensors
  photoValue = analogRead(photoPin);
  
  int index = -1;

  for(int i = 0; i < 3; i++) {
    if(photoValue <= timeIndex[i]) {
      index = i;
      break;
    }
  }

  if(index == -1) {
    index = 3;
  }

  int hour = -1;

  switch(index) {
    case 0:
      hour = 8;
      break;
    case 1:
      hour = 9;
      break;
    case 2:
      hour = 10;
      break;
    case 3:
      hour = 11;
      break;
  }

  float calibrated = 60 * ((photoValue - timeIndex[index - 1]) / ((float) timeIndex[index] - timeIndex[index - 1]));

  if((int) calibrated == 60) {
    calibrated = 0;
  }

  if(calibrated > 10) {
    Serial.print("The time is: ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print((int) calibrated);
    Serial.println(" AM");
  } else {
    Serial.print("The time is: ");
    Serial.print(hour);
    Serial.print(":0");
    Serial.print((int) calibrated);
    Serial.println(" AM");
  }  

  delay(60000);
}
