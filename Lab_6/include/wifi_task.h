#ifndef WIFI_TASK_H
#define WIFI_TASK_H

#include <Arduino.h>
#include <WiFi.h>

//void wifi_init_ap();

void wifi_init_sta();

void TaskWifi(void *pvParameter);

#endif