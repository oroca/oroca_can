# 03_CanGoogleTTS example need Only below action . 
*1. Add libraries to ESP32 ADF  
*2. Change platform.txt and add precompiled libraries  
```bash
You change Arduino15\packages\esp32\hardware\esp32\1.0.0-rc2\platform.txt. to use precompiled libraries like libesp_codec.a.  
I attach  platform.txt file at platform folder.    
And you add library file ( libesp_codec.a  and libesp-aac.a ) at Arduino15\packages\esp32\hardware\esp32\1.0.0-rc2\tools\sdk\lib .  
I attach  libray ( libesp_codec.a  and libesp-aac.a ) at platform folder.   
```
*3. You set your value at  translate_device_example.c .  
```bash
#define CONFIG_WIFI_SSID "G6"  // set your value  
#define CONFIG_WIFI_PASSWORD "555555555"  // set your value  
#define CONFIG_GOOGLE_API_KEY "djskljgkjagiAjdkajgkAdjkfjkadsjgijoag"  // set your value  
```
