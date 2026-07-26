#ifndef TASK_OLED_H
#define TASK_OLED_H

#include "global.h"

extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;

void OLED_init();

void TaskOled(void *pvParameter);

#endif