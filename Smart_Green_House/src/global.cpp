#include "global.h"

SemaphoreHandle_t xI2CMutex = NULL;
QueueHandle_t xLcdQueue = NULL;  // for LCD
QueueHandle_t xRelayQueue = NULL;

SensorData sensorData = {0.0, 0.0, 0, 0};