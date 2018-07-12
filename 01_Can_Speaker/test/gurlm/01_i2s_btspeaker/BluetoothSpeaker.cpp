// Copyright 2018 Evandro Luis Copercini
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "sdkconfig.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#if defined(CONFIG_BT_ENABLED) && defined(CONFIG_BLUEDROID_ENABLED)

#ifdef ARDUINO_ARCH_ESP32
#include "esp32-hal-log.h"
#endif

#include "BluetoothSpeaker.h"

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include <esp_log.h>

#ifdef ARDUINO_ARCH_ESP32
#include "esp32-hal-log.h"
#endif

#include "bt_app_core.h"
#include "bt_app_av.h"
#include "driver/i2s.h"
#include "MAX98357.h"

/* event for handler "bt_av_hdl_stack_up */
enum {
    BT_APP_EVT_STACK_UP = 0,
};

/* handler for bluetooth stack enabled events */
static void bt_av_hdl_stack_evt(uint16_t event, void *p_param);

const char * _spp_server_name = "ESP32_SPP_SERVER";

#define QUEUE_SIZE 256
static uint32_t _spp_client = 0;

static bool _init_bt(const char *deviceName)
{

  ////////////////////////////////////////////////////////////
     
    i2s_mode_t mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
    i2s_comm_format_t comm_fmt = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB);
    int use_apll = 0;
    
    i2s_config_t i2s_config;
    i2s_config.mode                 = mode;  
    i2s_config.sample_rate          = 8000, // 44100
    i2s_config.bits_per_sample      = I2S_BITS_PER_SAMPLE_16BIT;
    i2s_config.channel_format       = I2S_CHANNEL_FMT_RIGHT_LEFT;  // 2-channels
    i2s_config.communication_format = comm_fmt;
    i2s_config.dma_buf_count        = 32;                          // number of buffers, 128 max.
    i2s_config.dma_buf_len          = 64;                          // size of each buffer
    i2s_config.intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1;        // Interrupt level 1
    i2s_config.use_apll             = use_apll;

    i2s_pin_config_t pin_config;

    pin_config.bck_io_num   = GPIO_NUM_26;
    pin_config.ws_io_num    = GPIO_NUM_25;
    pin_config.data_out_num = GPIO_NUM_21;
    pin_config.data_in_num  = I2S_PIN_NO_CHANGE;
    /*pin_config.bck_io_num   = pin_bck;
    pin_config.ws_io_num    = pin_lrc;
    pin_config.data_out_num = pin_din;
    pin_config.data_in_num  = I2S_PIN_NO_CHANGE;*/

    //i2s_driver_install(config->i2s_num, &i2s_config, 1, &i2s_event_queue);
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL );
    i2s_set_pin(I2S_NUM_0, &pin_config);
  
  ////////////////////////////////////////////////////////////
  
    if (!btStarted() && !btStart()){
        log_e("%s initialize controller failed\n", __func__);
        return false;
    }
    
    esp_bluedroid_status_t bt_state = esp_bluedroid_get_status();
    if (bt_state == ESP_BLUEDROID_STATUS_UNINITIALIZED){
        if (esp_bluedroid_init()) {
            log_e("%s initialize bluedroid failed\n", __func__);
            return false;
        }
    }
    
    if (bt_state != ESP_BLUEDROID_STATUS_ENABLED){
        if (esp_bluedroid_enable()) {
            log_e("%s enable bluedroid failed\n", __func__);
            return false;
        }
    }     

    /* create application task */
    bt_app_task_start_up();

    /* Bluetooth device name, connection mode and profile set up */
    bt_app_work_dispatch(bt_av_hdl_stack_evt, BT_APP_EVT_STACK_UP, NULL, 0, NULL);    

    esp_bt_dev_set_device_name(deviceName);

    // the default BTA_DM_COD_LOUDSPEAKER does not work with the macOS BT stack
    esp_bt_cod_t cod;
    cod.major = 0b00001;
    cod.minor = 0b000100;
    cod.service = 0b00000010110;
    if (esp_bt_gap_set_cod(cod, ESP_BT_INIT_COD) != ESP_OK) {
        log_e("%s set cod failed\n", __func__);
        return false;
    }

    return true;
}


static void bt_av_hdl_stack_evt(uint16_t event, void *p_param)
{
    ESP_LOGD(BT_AV_TAG, "%s evt %d", __func__, event);
    switch (event) {
    case BT_APP_EVT_STACK_UP: {
        /* set up device name */
        char *dev_name = "ESP_SPEAKER";
        esp_bt_dev_set_device_name(dev_name);

        /* initialize A2DP sink */
        esp_a2d_register_callback(&bt_app_a2d_cb);
        esp_a2d_sink_register_data_callback(bt_app_a2d_data_cb);
        esp_a2d_sink_init();

        /* initialize AVRCP controller */
        esp_avrc_ct_init();
        esp_avrc_ct_register_callback(bt_app_rc_ct_cb);

        /* set discoverable and connectable mode, wait to be connected */
        esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
        break;
    }
    default:
        ESP_LOGE(BT_AV_TAG, "%s unhandled evt %d", __func__, event);
        break;
    }
}

static bool _stop_bt()
{
    if (btStarted()){
        esp_bluedroid_disable();
        esp_bluedroid_deinit();
        btStop();
    }
    return true;
}

/*
 * Bluetooth Speaker Arduino
 *
 * */

BluetoothSpeaker::BluetoothSpeaker()
{
    local_name = "ESP32"; //default bluetooth name
}

BluetoothSpeaker::~BluetoothSpeaker(void)
{
    _stop_bt();
}

bool BluetoothSpeaker::begin(String localName)
{
    if (localName.length()){
        local_name = localName;
    }
    return _init_bt(local_name.c_str());
}

int BluetoothSpeaker::available(void)
{
    return 0;
}

int BluetoothSpeaker::peek(void)
{
    return 0;
}

int BluetoothSpeaker::read(void)
{
    return 0;
}

size_t BluetoothSpeaker::write(uint8_t c)
{

    return 0;
}

void BluetoothSpeaker::flush()
{

}

void BluetoothSpeaker::end()
{
    _stop_bt();
}

#endif
