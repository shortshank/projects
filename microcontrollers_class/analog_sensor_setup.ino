/*
The very first project I made in this class, this code controlled an LED strip using a couple of different sensors.
*/

#include <Adafruit_NeoPixel.h> //library to control LED strip

const int NUMPIXELS = 10; //number of LEDs in 1m
const int LEDsPin = 9;    // LEDs connected to digital pin 9

const int bluePotPin = A0;   // pin to measure potentiometer
const int photoPin = A1; //pin that measures photoresistor
const int thermoPin = A2; //pin to measure thermistor

int bluePotValue = 0; // potentiometer value
int photoValue = 0; //value of photoresistor
int thermoValue = 0; //raw analog value of thermistor

int blueValue = 0; //potentiometer mapped value
int photoMapValue = 0; // mapped photoresistor value
int thermoMapValue = 0; // mapped thermistor value

//setup the NeoPixel library
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDsPin, NEO_GRB + NEO_KHZ800);

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // set the digital pin as output
  pinMode(LEDsPin, OUTPUT);
}

void loop() {
 
  // read the value from the various analog sensors
  bluePotValue = analogRead(bluePotPin);
  photoValue = analogRead(photoPin);
  thermoValue = analogRead(thermoPin);

  // print out the values to the serial monitor
  Serial.print("Raw sensor values: ");
  Serial.print(photoValue);
  Serial.println(bluePotValue);
  Serial.println(thermoValue);

  //map raw sensor values to a range of 0 to 255
  blueValue = map(bluePotValue, 0, 1023, 0, 255); // 0 to 1023, the maximum values achievable
  thermoMapValue = map(thermoValue, 300, 640, 0, 255); // the intial values are set to 300 and 640, which are approximately room temperature and just below boiling

  //first if statement only triggered if the photoresistor measures a light level below a certain value
  if(photoValue < 300) {  
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(thermoMapValue, 0, blueValue)); //potentiometer sets the blue value, thermistor sets red value of RGB
      pixels.show(); 
      delay(50); 
    }
  } else { // turn all LEDs off if the brightness is above a certain value
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(50);
    }
  }
}
