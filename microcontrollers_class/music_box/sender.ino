//music box sender

#include <esp_now.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

uint8_t receiverMAC[] = {0x80, 0xF3, 0xDA, 0x61, 0xB4, 0x78};
LiquidCrystal_I2C lcd(0x27, 16, 4); // Address 0x27, 16 chars, 2 lines

int x = 33;
int y = 32;
int b = 25;

int lastTrack = 1;
int lastVolume = 20;
int lastPause = LOW;

typedef struct {
  int track;
  int volume;
  bool paused;
} ControlData;

ControlData outgoingData;
esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  esp_now_init();
  
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
  
  outgoingData.track = 1;
  outgoingData.volume = 20;
  outgoingData.paused = false;

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Music Control");
  delay(1000);

  pinMode(b, INPUT_PULLDOWN);
  pinMode(x, INPUT);
  pinMode(y, INPUT);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Track: ");
  lcd.print(outgoingData.track);
  lcd.setCursor(0, 1);
  lcd.print("Volume: ");
  lcd.print(outgoingData.volume);
  lcd.setCursor(0, 2);
  lcd.print(outgoingData.paused ? "Paused" : "Playing");
}

void loop() {
  if(analogRead(x) < 2000 && outgoingData.volume < 30) {
    outgoingData.volume++;
  } else if(analogRead(x) > 3000 && outgoingData.volume > 0) {
    outgoingData.volume--;
  }

  if(analogRead(y) < 2000) {
    if(outgoingData.track == 1) {
      outgoingData.track = 10;
    } else {
      outgoingData.track--;
    }

    outgoingData.paused = false;
  } else if(analogRead(y) > 3000) {
    if(outgoingData.track == 10) {
      outgoingData.track = 1;
    } else {
      outgoingData.track++;
    }

    outgoingData.paused = false;
  }

  if(digitalRead(b) == HIGH && digitalRead(b) != lastPause) {
    lcd.setCursor(0, 2);
    outgoingData.paused = !outgoingData.paused;
    lcd.print(outgoingData.paused ? "Paused " : "Playing");
  }

  lastPause = digitalRead(b);

  if(outgoingData.track != lastTrack) {
    if(outgoingData.track == 10) {  
      lcd.setCursor(7, 0);
      lcd.print(outgoingData.track);
      lastTrack = outgoingData.track;
    } else {
      lcd.setCursor(7, 0);
      lcd.print(outgoingData.track);
      lcd.print(" ");
      lastTrack = outgoingData.track;
    }  
  }
  
  if(outgoingData.volume != lastVolume) {
    if(outgoingData.volume > 9) {  
      lcd.setCursor(8, 1);
      lcd.print(outgoingData.volume);
      lastVolume = outgoingData.volume;
    } else {
      lcd.setCursor(8, 1);
      lcd.print(outgoingData.volume);
      lcd.print(" ");
      lastVolume = outgoingData.volume;
    }
  }  

  Serial.println(analogRead(x));
  Serial.println(analogRead(y));
  Serial.println(digitalRead(b));
    
  esp_now_send(receiverMAC, (uint8_t *)&outgoingData, sizeof(outgoingData));

  delay(250);
}
