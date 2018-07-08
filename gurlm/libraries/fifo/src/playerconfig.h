#ifndef _PLAYER_CONFIG_H_
#define _PLAYER_CONFIG_H_

#define CONFIG_WIFI_SSID "test"
#define CONFIG_WIFI_PASSWORD "test"
#define CONFIG_AUDIO_OUTPUT_MODE I2S
//#define CONFIG_AUDIO_OUTPUT_MODE I2S_MERUS
//#define CONFIG_AUDIO_OUTPUT_MODE DAC_BUILT_IN
//#define CONFIG_AUDIO_OUTPUT_MODE PDM

// defined via 'make menuconfig'
#define WIFI_AP_NAME CONFIG_WIFI_SSID
#define WIFI_AP_PASS CONFIG_WIFI_PASSWORD


// defined via 'make menuconfig'
#define AUDIO_OUTPUT_MODE CONFIG_AUDIO_OUTPUT_MODE

#define FAKE_SPI_BUFF

#endif
