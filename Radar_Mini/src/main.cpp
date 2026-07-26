#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <ESP32Servo.h>
#include "global.h"
#include "servo_task.h"
#include "sensor_task.h"
#include "mqtt_task.h"
#include "wifi_task.h"
#include "oled_task.h"


void setup() {
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

	Serial.begin(115200);
	Wire.begin(SDA_I2C, SCL_I2C);

	xTaskCreate(TaskWifi, "WiFi", 4096, NULL, 3, NULL);
	xTaskCreate(TaskMqtt, "MQTT", 4096, NULL , 2, NULL);

	xQueueServo = xQueueCreate(10, sizeof(char*));
	if (xQueueServo != NULL) {
		xTaskCreate(TaskServo, "SERVO", 2048, NULL, 1, NULL);
	}

  	xQueueRadar = xQueueCreate(10, sizeof(Radar));
	if (xQueueRadar != NULL){
		xTaskCreate(TaskSensor, "SENSOR SONAR", 2048, NULL, 2, NULL);
	}
	
	xI2Cmutex = xSemaphoreCreateMutex();
	xTaskCreate(TaskOled, "OLED", 4096, NULL, 1, NULL);
}
void loop() {
	vTaskDelete(NULL);
}