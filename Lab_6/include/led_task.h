#ifndef LED_TASK_H
#define LED_TASK_H

#include <Arduino.h>

extern QueueHandle_t xQueueLed;

void TaskLed(void *pvParameter);

#endif