#include <Arduino.h>
#include "wifi_task.h"
//#include "webserver_task.h"
#include "led_task.h"
#include "mqtt_task.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  xQueueLed = xQueueCreate(5, sizeof(const char*));

  if (xQueueLed == nullptr){

    Serial.println("[Main] Failed to create LED queue");

    while (1){
      delay(1000);
    }
  }

  xTaskCreate(TaskWifi, "WIFI TASK", 4096, NULL, 2, NULL);
  //vTaskDelay(pdMS_TO_TICKS(15));
  //xTaskCreate(TaskWebServer, "WEB SERVER", 4096, NULL, 1, NULL);
  xTaskCreate(TaskMqtt, "MQTT TASK", 4096, NULL, 1, NULL);
  xTaskCreate(TaskLed, "LED TASK", 2048, NULL, 1, NULL);

  Serial.println("[Main] All Task started!");
}

void loop() {
  // put your main code here, to run repeatedly:
}