#include <servo_task.h>

Servo servo;

void TaskServo(void *pvParameter){

	servo.setPeriodHertz(50);
	servo.attach(PWM_PIN, 500, 2400);

	int angle = 0;
	int step = 1;

	const char* cmd;
	bool isScanning = false;

	while(1){

		if (xQueueReceive(xQueueServo, &cmd, 0)){
			if (strcmp(cmd, "ON") == 0){
				isScanning = true;
			} else if (strcmp(cmd, "OFF") == 0){
				isScanning = false;
			}
		}

		if (isScanning == true){

			servo.write(angle);

			angle += step;

			if (angle >= 180){
				step = -1;
			}

			if (angle <= 0){
				step = 1;
			}

			glob_angle = angle;
		}

		vTaskDelay(pdMS_TO_TICKS(15));
	}
}