#ifndef _CANSPEAKER_H_
#define _CANSPEAKER_H_


#include "Arduino.h"


#include "./src/bt_speaker/bt_speaker.h"


class CanSpeaker
{
  public:
    CanSpeaker();
    ~CanSpeaker();    

    void begin(char *speaker_name);

  private:    
    renderer_config_t *renderer_config;
};


#endif

