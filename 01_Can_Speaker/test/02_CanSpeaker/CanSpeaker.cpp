#include "CanSpeaker.h"



extern char CONFIG_BT_NAME[128];


CanSpeaker::CanSpeaker(void)
{
}

CanSpeaker::~CanSpeaker()
{
}

void CanSpeaker::begin(char *speaker_name)
{
  renderer_config = (renderer_config_t *)calloc(1, sizeof(renderer_config_t));

  renderer_config->bit_depth = I2S_BITS_PER_SAMPLE_16BIT;
  renderer_config->i2s_num = I2S_NUM_0;
  renderer_config->sample_rate = 44100;
  renderer_config->sample_rate_modifier = 1.0;
  renderer_config->output_mode = I2S;

  strcpy(CONFIG_BT_NAME, speaker_name);

  bt_speaker_start(renderer_config);
}

