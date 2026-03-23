//music box receiver, that actually played the music

#include <esp_now.h>
#include <WiFi.h>
#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>

HardwareSerial dfSerial(1);
DFRobotDFPlayerMini dfPlayer;

typedef struct {
  int track;
  int volume;
  bool paused;
} ControlData;

ControlData incomingData;

int lastVolume = -1;
int lastTrack = -1;
bool pauseStatus = false;

void onDataRecv(const esp_now_recv_info* recv_info, const uint8_t* data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));
  
  Serial.println("=== RECEIVED DATA ===");
  Serial.print("Track: ");
  Serial.println(incomingData.track);
  Serial.print("Volume: ");
  Serial.println(incomingData.volume);
  Serial.print("Paused: ");
  Serial.println(incomingData.paused ? "YES" : "NO");
  Serial.println("====================");

  if(incomingData.volume != lastVolume) {
    dfPlayer.volume(incomingData.volume);
    lastVolume = incomingData.volume;
  }

  if(incomingData.track != lastTrack) {
    dfPlayer.play(incomingData.track);
    lastTrack = incomingData.track;
  }

  if(incomingData.paused != pauseStatus) {
    pauseStatus = incomingData.paused;
    if(pauseStatus) {
      dfPlayer.pause();
    } else {
      dfPlayer.start();
    }
  }
}

void setup() {
  Serial.begin(115200);

  dfSerial.begin(9600, SERIAL_8N1, 16, 17);
  dfPlayer.begin(dfSerial);
  dfPlayer.volume(20);
  
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(onDataRecv);
}

void loop() {

}
