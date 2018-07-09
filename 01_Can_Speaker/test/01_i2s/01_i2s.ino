#include "MAX98357.h"
#include "wave_file.h"



MAX98357 speaker;


static pcm_format_t bt_buffer_fmt = {
    .sample_rate   = 8000, // 44100
    .bit_depth     = I2S_BITS_PER_SAMPLE_16BIT,
    .num_channels  = 2,
    .buffer_format = PCM_INTERLEAVED
};


void setup() {

  Serial.begin(115200);
  speaker.begin();    
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
}



