#include <Arduino.h>

// put function declarations here:
void Task1(void *pv){
  while(true){
    Serial.println("Hello World!\n");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  xTaskCreate(Task1, "Task: Hello World", 4096, NULL, 1, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}
