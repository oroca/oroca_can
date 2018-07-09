
#include "bt_speaker.h"
#include "playerconfig.h"

void setup() {

    Serial.begin(115200);
    Serial.println("starting setup()");

    bt_speaker_start(create_renderer_config());

}

void loop() {
    delay(20);
}

static renderer_config_t *create_renderer_config()
{
    renderer_config_t *renderer_config = (renderer_config_t *)calloc(1, sizeof(renderer_config_t));

    renderer_config->bit_depth = I2S_BITS_PER_SAMPLE_16BIT;
    renderer_config->i2s_num = I2S_NUM_0;
    renderer_config->sample_rate = 44100;
    renderer_config->sample_rate_modifier = 1.0;
    renderer_config->output_mode = AUDIO_OUTPUT_MODE;

    if(renderer_config->output_mode == I2S_MERUS) {
        renderer_config->bit_depth = I2S_BITS_PER_SAMPLE_32BIT;
    }

    if(renderer_config->output_mode == DAC_BUILT_IN) {
        renderer_config->bit_depth = I2S_BITS_PER_SAMPLE_16BIT;
    }
    return renderer_config;
}

