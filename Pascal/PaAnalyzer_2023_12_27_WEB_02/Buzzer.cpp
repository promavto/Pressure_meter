
#include "Buzzer.h"
//--------------------------------------------------------------------------------------------------------------------------------------
BuzzerClass Buzzer;
//--------------------------------------------------------------------------------------------------------------------------------------


hw_timer_t* timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

BuzzerClass::BuzzerClass()
{
  active = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------
void BuzzerClass::begin()
{
  pinMode(BUZZER_PIN,OUTPUT);
  digitalWrite(BUZZER_PIN,!BUZZER_LEVEL);
  

  // Create semaphore to inform us when the timer has fired
  timerSemaphore = xSemaphoreCreateBinary();

  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  timer = timerBegin(0, 80, true);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &buzzOffHandler, true);
 // timerAttachInterrupt(timer, &onTimer, true);

  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timer, BUZZER_DURATION, true);
  //timerAlarmWrite(timer, 60000, true);
  
  // Start an alarm
  timerAlarmEnable(timer);

}
//--------------------------------------------------------------------------------------------------------------------------------------
void BuzzerClass::stop()
{
  buzzLevel(false);
  timerStop(timer);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void BuzzerClass::buzz()
{
  if(active)
    return;

  active = true;
  buzzLevel(true);
  timerStart(timer);
 }
//--------------------------------------------------------------------------------------------------------------------------------------
void BuzzerClass::buzzLevel(bool on)
{
  digitalWrite(BUZZER_PIN, on ? BUZZER_LEVEL : !BUZZER_LEVEL);
}
//--------------------------------------------------------------------------------------------------------------------------------------
void BuzzerClass::buzzOffHandler()//(void* param)
{
    portENTER_CRITICAL_ISR(&timerMux);
   // interruptCounter++;
    portEXIT_CRITICAL_ISR(&timerMux);

  timerStop(timer);
  Buzzer.buzzLevel(false);
  Buzzer.active = false;
 
}
//--------------------------------------------------------------------------------------------------------------------------------------
