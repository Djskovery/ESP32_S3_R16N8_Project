#ifndef WEBSERVER_TASK_H
#define WEBSERVER_TASK_H

#include <Arduino.h>
#include <WebServer.h>

extern QueueHandle_t xQueueLed;

void webserver_init();
void TaskWebServer(void *pvParameter);

#endif