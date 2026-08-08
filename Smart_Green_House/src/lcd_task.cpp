#include "lcd_task.h"

void DisplayTask(void *pvParameter){

    LiquidCrystal_I2C lcd(0x21, 16, 2);

    if (xSemaphoreTake(xI2CMutex, portMAX_DELAY) == pdTRUE){
        lcd.begin();
        lcd.backlight();
        xSemaphoreGive(xI2CMutex);
    }

    SensorData localData;

    char row0_buffer[17];
    char row1_buffer[17];

    while(true){

        if (xQueueReceive(xLcdQueue, &localData, portMAX_DELAY)){
            snprintf(row0_buffer, sizeof(row0_buffer), "T: %.1f L: %d", localData.temp, localData.light);
            snprintf(row1_buffer, sizeof(row1_buffer), "H: %.1f M: %d", localData.humid, localData.moisture);

            if (xSemaphoreTake(xI2CMutex, pdMS_TO_TICKS(100)) == pdTRUE){
            lcd.setCursor(0, 0);
            lcd.print(row0_buffer);

            lcd.setCursor(0, 1);
            lcd.print(row1_buffer);

            xSemaphoreGive(xI2CMutex);
        }
        }
    }
}