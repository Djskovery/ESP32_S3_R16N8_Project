#include "sensors_task.h"

void SensorsTask(void *pvParameter){

    pinMode(MOISTURE_PIN, INPUT);

    DHT20    dht20;
    VEML6040 colorSensor;

    if (xSemaphoreTake(xI2CMutex, portMAX_DELAY) == pdTRUE){
        dht20.begin();
        colorSensor.begin();
        colorSensor.setConfiguration(0);
        xSemaphoreGive(xI2CMutex);
    }

    SensorData localData;

    while(true){

        if (xSemaphoreTake(xI2CMutex, pdMS_TO_TICKS(100)) == pdTRUE){
            dht20.read();
            localData.temp = dht20.getTemperature();
            localData.humid = dht20.getHumidity();
            localData.light = colorSensor.getWhite();

            xSemaphoreGive(xI2CMutex);
        }

        int rawMoisture = analogRead(MOISTURE_PIN);
        int percent = map(rawMoisture, MOISTURE_DRY, MOISTURE_WET, 0, 100);

        localData.moisture = constrain(percent, 0, 100);

        xQueueSend(xLcdQueue, &localData, 0);
        xQueueSend(xRelayQueue, &localData, 0);

        vTaskDelay(pdMS_TO_TICKS(3000));

    }
}