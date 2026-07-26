#include <sensor_task.h>

void TaskSensor(void *pvParameter){

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Radar radar;

    while(1){

        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);

        int duration = pulseIn(ECHO_PIN, HIGH, 30000);
        if (duration > 0){
            radar.distance = duration * 0.034 / 2;
        } else {
            radar.distance = -1;
        }

        radar.angle = glob_angle;
        glob_distance = radar.distance;

        xQueueSend(xQueueRadar, &radar, 0);

        vTaskDelay(pdMS_TO_TICKS(30));
    }
}