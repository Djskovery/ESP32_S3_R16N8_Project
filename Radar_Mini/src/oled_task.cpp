#include "oled_task.h"

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define OLED_W 128
#define OLED_H 64

static const float RADAR_DISPLAY_MAX_CM = 40.0f;
static const int   RADAR_CX     = OLED_W / 2;
static const int   RADAR_CY     = OLED_H - 1; 
static const int   RADAR_TOP_Y  = 9;                
static const float RADAR_RADIUS = (float)min(RADAR_CX - 2, RADAR_CY - RADAR_TOP_Y);

static void drawArc(float r) {
    u8g2.drawCircle(RADAR_CX, RADAR_CY, (uint16_t)r,
                     U8G2_DRAW_UPPER_LEFT | U8G2_DRAW_UPPER_RIGHT);
}

static void drawSpoke(float aDeg, float r) {
    float rad = radians(aDeg);
    int x = RADAR_CX + (int)(r * cos(rad));
    int y = RADAR_CY - (int)(r * sin(rad));
    u8g2.drawLine(RADAR_CX, RADAR_CY, x, y);
}

void TaskOled(void *pvParameter){

    u8g2.begin();
    u8g2.setContrast(128);

    while(true){

        int   angle = glob_angle;
        float dist  = glob_distance;
        bool  inRange = (dist > 0 && dist <= RADAR_DISPLAY_MAX_CM);

        if (xSemaphoreTake(xI2Cmutex, pdMS_TO_TICKS(100)) == pdTRUE){

            u8g2.clearBuffer();

            u8g2.setFont(u8g2_font_5x7_tf);
            char buf[32];
            if (inRange) {
                snprintf(buf, sizeof(buf), "A:%3d  D:%dcm", angle, (int)dist);
            } else {
                snprintf(buf, sizeof(buf), "A:%3d  D:--", angle);
            }
            u8g2.drawStr(0, 7, buf);

            for (int i = 1; i <= 3; i++) {
                drawArc(RADAR_RADIUS * i / 3.0f);
            }
            for (int a = 0; a <= 180; a += 45) {
                drawSpoke((float)a, RADAR_RADIUS);
            }

            float rad = radians((float)angle);
            int sx = RADAR_CX + (int)(RADAR_RADIUS * cos(rad));
            int sy = RADAR_CY - (int)(RADAR_RADIUS * sin(rad));
            u8g2.drawLine(RADAR_CX, RADAR_CY, sx, sy);
            u8g2.drawLine(RADAR_CX, RADAR_CY - 1, sx, sy - 1);

            if (inRange) {
                float rObj = (dist / RADAR_DISPLAY_MAX_CM) * RADAR_RADIUS;
                int ox = RADAR_CX + (int)(rObj * cos(rad));
                int oy = RADAR_CY - (int)(rObj * sin(rad));
                u8g2.drawDisc(ox, oy, 2);
            }

            u8g2.sendBuffer();

            xSemaphoreGive(xI2Cmutex);
        }

        vTaskDelay(pdMS_TO_TICKS(80));
    }
}