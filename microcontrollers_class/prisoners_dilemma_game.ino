/*
First actual project. This is code for a two-player game, featuring the (in)famous Prisoner's Dilemma. 
The original version was a singleplayer version, but I failed to save the code, and it is now lost to time.
Image in this repo shows the physical setup.
*/

//package setup
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//define pins and such
const int numPixels = 9;
const int p1LED = 4;
const int p2LED = 5;
const int p1Touch = 7;
const int p2Touch = 8;

//player score variables
int p1Score = 0;
int p2Score = 0;

//player choice variables - true means betray, false is cooperate
bool p1Betray = false;
bool p2Betray = false;

//set up LCD and LEDs
Adafruit_NeoPixel p1 = Adafruit_NeoPixel(9, p1LED, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel p2 = Adafruit_NeoPixel(9, p2LED, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);
  reset(); //reset all variables
  
  //initialize LED strips
  p1.begin();
  p2.begin();
  
  //initialize pins
  pinMode(p1LED, OUTPUT);
  pinMode(p2LED, OUTPUT);
  pinMode(p1Touch, INPUT);
  pinMode(p2Touch, INPUT);
  
  //initialize LED strips
  lcd.init();
  lcd.backlight();
}

void loop() {
  gameStart(); //prompts users to hold the buttons to start game
  delay(1000); //1 second delay to deconflict start game inputs from game choices
  while(true) {  
    getInput(); //prompt both users to tap to betray, nothing to cooperate, time limit of 3s
    getScore(); //take input, process score, show choices on LEDs (green for cooperate, red for betray)
    showScore(); //calculate score and print to LCD
    if(checkGame()) { //if either player has reached 8, end the game, if a player has gone over 8, reset their score if the score is 8-8, reset the score
      delay(5000);
      reset(); //reset all variables
      break; //restart loop()
    }
    loopReset(); //resets certain variables/outputs - LCD, LED, choices
  }  
}

//this method resets all variables and displays
void reset() {
  p1Score = 0;
  p2Score = 0;
  loopReset(); //call loopReset() to avoid repetition
  
  p1.clear();
  p2.clear();
  p1.show();
  p2.show();
}

//this method resets certain variables within the game loop, namely choices and clearing displays
void loopReset() {
  p1Betray = false;
  p2Betray = false;

  p1.clear();
  p1.show();
  p2.clear();
  p2.show();
  lcdReset();
}

//helper method written to make resetting an LCD easier
void lcdReset() {
  lcd.clear();
  lcd.setCursor(0, 0);
}

//method to start game
void gameStart() {
  lcd.print("Hold your button to start..."); //prompt players to press their buttons
  while(true) { //check if buttons are held
    if(digitalRead(p1Touch) == HIGH && digitalRead(p2Touch) == HIGH) {
      break;
    }
  }

  lcdReset();
}

void getInput() {
  lcd.print("Make your choice..."); //prompt for choice
  unsigned long time = millis(); //first recorded time to impose time limit on choice
  lcd.setCursor(0, 3);
  //print the time counting down
  lcd.print("3...");
  while(millis() - time < 3000) {
    if(millis() - time == 1000) {
      lcd.print("2...");
    } else if(millis() - time == 2000) {
      lcd.print("1...");
    }

    //checking if the player chooses to betray
    if(digitalRead(p1Touch) == HIGH) {
      p1Betray = true;
    }
    
    if(digitalRead(p2Touch) == HIGH) {
      p2Betray = true;
    }
  }

  lcdReset();
}

//calculate score from choices using if-else statements, then show the choices on the LED
void getScore() {
  //score calculations
  if(p1Betray == false && p2Betray == false) {
    p1Score += 3;
    p2Score += 3;
  } else if(!p1Betray && p2Betray) {
    p1Score++;
    p2Score += 5;
  } else if(p1Betray && !p2Betray) {
    p1Score += 5;
    p2Score++;
  } else if(p1Betray == true && p2Betray == true) {
    p1Score++;
    p2Score++;
  }

  //show choices on LED strip
  if(p1Betray) {
    for(int i = 0; i < 9; i++) {
      p1.setPixelColor(i, p1.Color(180, 0, 0));
    }

    p1.show();
  } else {
    for(int i = 0; i < 9; i++) {
      p1.setPixelColor(i, p1.Color(0, 180, 0));
    }
    
    p1.show();
  }

  if(p2Betray) {
    for(int i = 0; i < 9; i++) {
      p2.setPixelColor(i, p2.Color(180, 0, 0));
    }

    p2.show();
  } else {
    for(int i = 0; i < 9; i++) {
      p2.setPixelColor(i, p2.Color(0, 180, 0));
    }

    p2.show();
  }

  lcdReset();
}

//show the score on the LCD
void showScore() {
  lcdReset();
  lcd.print("Score:");
  lcd.setCursor(0, 1);
  lcd.print("Player 1: ");
  lcd.print(p1Score);
  lcd.setCursor(0, 2);
  lcd.print("Player 2: ");
  lcd.print(p2Score);

  delay(3000);
}

//check the game status - return true if someone has won, otherwise return false
bool checkGame() {
  lcdReset();
  //check if someone has won
  if(p1Score == 8 && p2Score != 8) {
    lcd.print("Player 1 Won! Score:");
    lcd.setCursor(0, 1);
    lcd.print("Player 1: ");
    lcd.print(p1Score);
    lcd.setCursor(0, 2);
    lcd.print("Player 2: ");
    lcd.print(p2Score);
    return true;
  } else if(p2Score == 8 && p1Score != 8) {
    lcd.print("Player 2 Won! Score:");
    lcd.setCursor(0, 1);
    lcd.print("Player 1: ");
    lcd.print(p1Score);
    lcd.setCursor(0, 2);
    lcd.print("Player 2: ");
    lcd.print(p2Score);
    return true;
  }

  //check if someone's score is over the limit of 8
  if(p1Score > 8) {
    lcdReset();
    lcd.print("Player 1 score is ");
    lcd.setCursor(0, 1);
    lcd.print("too high! Resetting.");
    p1Score = 0;
    
    delay(1000);
  }

  if(p2Score > 8) {
    lcdReset();
    lcd.print("Player 2 score is ");
    lcd.setCursor(0, 1);
    lcd.print("too high! Resetting.");
    p2Score = 0;

    delay(1000);
  }

  //check if the players have tied
  if(p1Score == p2Score && p1Score == 8) {
    lcdReset();
    lcd.print("Tied! Resetting...");
    p1Score = 0;
    p2Score = 0;
  }

  lcdReset();
  return false;
}
