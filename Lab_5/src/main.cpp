#include <Arduino.h>

TimerHandle_t xTimer1;
TimerHandle_t xTimer2;

void ShareTimerCallback(TimerHandle_t xTimer){

  uint32_t xTickCountID;

  xTickCountID = (uint32_t) pvTimerGetTimerID(xTimer);

  xTickCountID++;

  vTimerSetTimerID(xTimer, (void *)xTickCountID);

  if (xTimer == xTimer1){
    
    Serial.printf("%d ahihi\n", xTickCountID);

    if (xTickCountID == 10){
      xTimerStop(xTimer1, 0);
    }

  } else if (xTimer == xTimer2){

    Serial.printf("%d ihaha\n", xTickCountID);

    if (xTickCountID == 5){
      xTimerStop(xTimer2, 0);
    }

  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  xTimer1 = xTimerCreate("TIMER_1", pdMS_TO_TICKS(2000), pdTRUE, (void *)0, ShareTimerCallback);
  xTimer2 = xTimerCreate("TIMER_2", pdMS_TO_TICKS(3000), pdTRUE, (void *)0, ShareTimerCallback);

  if (xTimer1 != NULL && xTimer2 != NULL){

    xTimerStart(xTimer1, 0);
    xTimerStart(xTimer2, 0);

  }
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelete(NULL);
}
