#include "relay_task.h"

void ControlDevicesTask(void *pvParameter){

    Multi_Channel_Relay relay;

    if (xSemaphoreTake(xI2CMutex, portMAX_DELAY) == pdTRUE){
        relay.begin();
        xSemaphoreGive(xI2CMutex);
    }

    SensorData localData;

    static uint8_t relay_state = 0; 

    while (true){
        
        if (xQueueReceive(xRelayQueue, &localData, portMAX_DELAY) == pdTRUE){
            
            if (localData.temp >= 30.0){
                bitSet(relay_state, 2);
            } else {
                bitClear(relay_state, 2);
            }

            if (localData.moisture < 80){
                bitSet(relay_state, 3);
            } else {
                bitClear(relay_state, 3);
            }

            if (localData.light < 1500){
                bitSet(relay_state, 0);
                bitSet(relay_state, 1);
            } else if (localData.light >= 2000){
                bitClear(relay_state, 0);
                bitClear(relay_state, 1);
            }

            if (xSemaphoreTake(xI2CMutex, pdMS_TO_TICKS(100)) == pdTRUE){
                relay.channelCtrl(relay_state); 
                xSemaphoreGive(xI2CMutex);
            }
        }
    }
}