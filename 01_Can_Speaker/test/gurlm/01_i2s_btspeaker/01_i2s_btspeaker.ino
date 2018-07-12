//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial
#include "MAX98357.h"
#include "BluetoothSpeaker.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

static pcm_format_t bt_buffer_fmt = {
    .sample_rate   = 8000, // 44100
    .bit_depth     = I2S_BITS_PER_SAMPLE_16BIT,
    .num_channels  = 2,
    .buffer_format = PCM_INTERLEAVED
};

BluetoothSpeaker SpeakerBT;
MAX98357 speaker;

void setup() {
  
  Serial.begin(115200);  
  //speaker.begin();
  SpeakerBT.begin("ESP32speaker"); //Bluetooth device name  
  Serial.println("The device started, now you can pair it with bluetooth!");
  
}

void loop() {
  delay(20);
}
