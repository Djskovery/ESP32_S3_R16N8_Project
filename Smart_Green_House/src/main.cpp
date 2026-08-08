#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include "global.h"
#include "sensors_task.h"
#include "lcd_task.h"
#include "relay_task.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(SDA_I2C, SCL_I2C);

  xI2CMutex = xSemaphoreCreateMutex();
  xLcdQueue = xQueueCreate(10, sizeof(SensorData));
  xRelayQueue = xQueueCreate(10, sizeof(SensorData));

  if (xLcdQueue != NULL && xI2CMutex != NULL){
    xTaskCreate(SensorsTask, "Sensors Data", 4096, NULL, 2, NULL);
    xTaskCreate(DisplayTask, "Display Data", 4096, NULL, 1, NULL);
  }

  if (xRelayQueue != NULL && xI2CMutex != NULL){
    xTaskCreate(ControlDevicesTask, "Control Devices", 4096, NULL, 1, NULL);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
}

