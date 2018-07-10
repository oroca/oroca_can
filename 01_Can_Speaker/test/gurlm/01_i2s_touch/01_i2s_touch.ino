
#include "MAX98357.h"
#include "wave_file.h"
#include "esp_avrc_api.h"
#include "bt_speaker.h"

MAX98357 speaker;

static pcm_format_t bt_buffer_fmt = {
    .sample_rate   = 8000, // 44100
    .bit_depth     = I2S_BITS_PER_SAMPLE_16BIT,
    .num_channels  = 2,
    .buffer_format = PCM_INTERLEAVED
};

//gurlm start///////////////////////

int threshold = 40;
bool touch1detected = false;
bool touch2detected = false;
bool touch1state = false;
bool touch2state = false;
int transaction_label = 5 ;

void gotTouch1(){
 touch1detected = true;
}

void gotTouch2(){
 touch2detected = true;
}
//gurlm end/////////////////////////


void setup() {

  Serial.begin(115200);
  speaker.begin();
  //gurlm start///////////////////////
  bt_speaker_start();
  touchAttachInterrupt(T2, gotTouch1, threshold);
  touchAttachInterrupt(T3, gotTouch2, threshold);  
  //gurlm end/////////////////////////
      
}

void loop() {
  if (Serial.available() > 0)
  {
    uint8_t ch;

    ch = Serial.read();

    if (ch == '1')
    {
      Serial.println("Start..");
      speaker.start();
      speaker.write((char *)data, NUM_ELEMENTS*2, &bt_buffer_fmt);
      delay(200);
      speaker.stop();
      Serial.println("End..");
    }  
  }

  //gurlm start///////////////////////

  if(touch1state){
    esp_avrc_ct_send_passthrough_cmd(transaction_label, ESP_AVRC_PT_CMD_PLAY, ESP_AVRC_PT_CMD_STATE_RELEASED);
  }
  if(touch2state){
    esp_avrc_ct_send_passthrough_cmd(transaction_label, ESP_AVRC_PT_CMD_STOP, ESP_AVRC_PT_CMD_STATE_RELEASED);
  }
  
  if(touch1detected){
    touch1detected = false;
    Serial.println("Touch 1 detected");
    esp_avrc_ct_send_passthrough_cmd(transaction_label, ESP_AVRC_PT_CMD_PLAY, ESP_AVRC_PT_CMD_STATE_PRESSED);
    touch1state = true ;    
  }  
  if(touch2detected){
    touch2detected = false;
    Serial.println("Touch 2 detected");
    esp_avrc_ct_send_passthrough_cmd(transaction_label, ESP_AVRC_PT_CMD_STOP, ESP_AVRC_PT_CMD_STATE_PRESSED);
    touch2state = true ; 
  }  
  //gurlm end/////////////////////////
    
}



