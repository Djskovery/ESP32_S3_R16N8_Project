#ifndef MQTT_TASK_H
#define MQTT_TASK_H

#include <Arduino.h>
#include <PubSubClient.h>

extern QueueHandle_t xQueueLed;

void mqtt_init();

void TaskMqtt(void *pvParameter);

#endif