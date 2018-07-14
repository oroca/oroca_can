//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial
#include "MAX98357.h"
#include "BluetoothSpeaker.h"
#include "esp_avrc_api.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

int threshold = 40;
int touch1count = 5;
int touch2count = 5;
bool touch1state = false;
bool touch2state = false;
bool prevtouch1state = false;
bool prevtouch2state = false;
int transaction_label = 5 ;

BluetoothSpeaker SpeakerBT;
MAX98357 speaker;

void gotTouch1(){
 touch1count = 100;
}

void gotTouch2(){
 touch2count = 100;
}

void setup() {
  
  Serial.begin(115200);  
  //speaker.begin();
  SpeakerBT.begin(); 
  touchAttachInterrupt(GPIO_NUM_14, gotTouch1, threshold);
  touchAttachInterrupt(GPIO_NUM_15, gotTouch2, threshold);  
  
  Serial.println("The device started, now you can pair it with bluetooth!");
  
}

void loop() {  

  touch_button_check();
  delay(10);
  
}

void touch_button_check() { 
  
  if( touch1count > 0 ){
    touch1count--;
    touch1state = true  ;
  }else{
    touch1state = false ;
  }

  if( touch2count > 0 ){
    touch2count--;
    touch2state = true  ;
  }else{
    touch2state = false ;
  }
  
  if( touch1state && !prevtouch1state ){    
    Serial.println("Touch 1 Pressed");
    SpeakerBT.remoteControl(transaction_label, ESP_AVRC_PT_CMD_PLAY, ESP_AVRC_PT_CMD_STATE_PRESSED);
  }else if( !touch1state && prevtouch1state ){    
    Serial.println("Touch 1 Released");
    SpeakerBT.remoteControl(transaction_label, ESP_AVRC_PT_CMD_PLAY, ESP_AVRC_PT_CMD_STATE_RELEASED);
  }

  if( touch2state && !prevtouch2state ){    
    Serial.println("Touch 2 Pressed");
    SpeakerBT.remoteControl(transaction_label, ESP_AVRC_PT_CMD_STOP, ESP_AVRC_PT_CMD_STATE_PRESSED);
  }else if( !touch2state && prevtouch2state ){    
    Serial.println("Touch 2 Released");
    SpeakerBT.remoteControl(transaction_label, ESP_AVRC_PT_CMD_STOP, ESP_AVRC_PT_CMD_STATE_RELEASED);
  }

  prevtouch1state = touch1state;
  prevtouch2state = touch2state;
    
}
