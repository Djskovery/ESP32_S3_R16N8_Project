#include <Arduino.h>

void TaskA(void *pvParameter){
  // Highest priority with Delay
  TickType_t lastWake = xTaskGetTickCount();
  while(1){

    Serial.println("A is running...");

    vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(500));
  }
}

void TaskB(void *pvParamter){
  while(1){
    Serial.println("B is running...");

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void TaskC(void *pvParameter){
  while(1){
    Serial.println("C is running...");

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void Task1(){
  Serial.begin(115200);

  xTaskCreate(TaskA, "A", 2048, NULL, 3, NULL);
  xTaskCreate(TaskB, "B", 2048, NULL, 1, NULL);
  xTaskCreate(TaskC, "C", 2048, NULL, 1, NULL);
  
}