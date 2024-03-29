#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------
#include <Arduino.h>
#include "Configuration_ESP32.h"

//--------------------------------------------------------------------------------------------------------------------------------------
class BuzzerClass
{
  public:
    BuzzerClass();
    void begin();
    void buzz();
    void stop();

   private:
   
    static void buzzOffHandler();//void* param);
    void buzzLevel(bool on);
    bool active;
  
};
//--------------------------------------------------------------------------------------------------------------------------------------
extern BuzzerClass Buzzer;

