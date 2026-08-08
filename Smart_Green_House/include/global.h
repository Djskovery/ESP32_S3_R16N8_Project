#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <Arduino.h>
#include <freertos/queue.h>
#include <semphr.h>
#include <stdio.h>
#include <sensor.h>
#include <WiFi.h>
#include <DHT20.h>
#include <LiquidCrystal_I2C.h>
#include <veml6040.h>
#include <multi_channel_relay.h>

#define SCL_I2C 12
#define SDA_I2C 11
#define MOISTURE_PIN 1

#define MOISTURE_DRY 4000
#define MOISTURE_WET 1200

extern SemaphoreHandle_t xI2CMutex;
extern QueueHandle_t xLcdQueue;  // for LCD
extern QueueHandle_t xRelayQueue; // for Relay

typedef struct {
    float temp;
    float humid;
    int   light;
    int   moisture;
} SensorData;

extern SensorData sensorData;

#endif