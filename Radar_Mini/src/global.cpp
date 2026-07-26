#include <global.h>

#define TRIG_PIN 6
#define ECHO_PIN 5
#define PWM_PIN  4
#define LED_RED  47
#define LED_GREEN 48
#define SCL_I2C 9
#define SDA_I2C 8

QueueHandle_t xQueueRadar = NULL;
QueueHandle_t xQueueServo = NULL;
SemaphoreHandle_t xI2Cmutex = NULL;

volatile int glob_angle = 0;
volatile float glob_distance = 0;

volatile float glob_ax, glob_ay, glob_az;
volatile float glob_gx, glob_gy, glob_gz;