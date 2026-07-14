#include "led_task.h"

static const int LED_PIN = 48;

QueueHandle_t xQueueLed;

void TaskLed(void *pvParameter){

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.println("[LED] LED Task started");
    const char* cmd;

    while(1){

        if (xQueueReceive(xQueueLed, &cmd, portMAX_DELAY)){
            
            if (strcmp(cmd, "ON") == 0){
                digitalWrite(LED_PIN, HIGH);
                Serial.println("[LED] LED ON");
            } else if (strcmp(cmd, "OFF") == 0){
                digitalWrite(LED_PIN, LOW);
                Serial.println("[LED] LED OFF");
            }
        }
    }
}