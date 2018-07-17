#include "CanSpeaker.h"


CanSpeaker speaker;


void setup() {
  Serial.begin(115200);

  speaker.begin("CanSpeaker(Baram)");
}

void loop() {
  static uint32_t pre_time = 0;

  if (millis()-pre_time >= 500)
  {
    pre_time = millis();
    Serial.println("Tick");
  }
}
