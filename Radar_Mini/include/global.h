#ifndef GLOBAL_H   
#define GLOBAL_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <ESP32Servo.h>
#include <freertos/queue.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <stdio.h>
#include <MPU6500_WE.h>
#include <Sensor.h>
#include <semphr.h>

#define TRIG_PIN 6
#define ECHO_PIN 5
#define PWM_PIN  4
#define LED_RED  47
#define LED_GREEN 48  
#define SCL_I2C 9
#define SDA_I2C 8


typedef struct {
    int   angle;
    float distance;
} Radar;

extern QueueHandle_t xQueueRadar;
extern QueueHandle_t xQueueServo;
extern SemaphoreHandle_t xI2Cmutex;

extern volatile int glob_angle;
extern volatile float glob_distance;

extern volatile float glob_ax, glob_ay, glob_az;
extern volatile float glob_gx, glob_gy, glob_gz;

#endif