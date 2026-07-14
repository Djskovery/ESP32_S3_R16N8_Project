#include <Arduino.h>
#include <string.h>

typedef struct {

  int32_t targetID;
  int32_t dataValue;

} Data_t;

QueueHandle_t xQueue;

// RECEPTION TASK
void TaskReception(void *pvParameter){

  while(1){

    if (Serial.available() > 0){

      String input = Serial.readStringUntil('\n');

      input.trim();

      if (input.length() > 0){

        Data_t requestData;

        int matched = sscanf(input.c_str(), "%d %d", &requestData.targetID, &requestData.dataValue);

        if (matched == 2){

          BaseType_t xStatus = xQueueSend(xQueue, &requestData, 0);

          if (xStatus == pdPASS){
            Serial.printf("Sent to Queue -> ID: %d, Value: %d\n", requestData.targetID, requestData.dataValue);
          } else {
            Serial.println("Fault!, Queue is full.\n");
          }
        } else {
          Serial.println("Failed!, type again\n");
        }
      } 
    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}


void TaskFunc1(void *pvParameter){

  Data_t infor;

  while(1){

    BaseType_t xStatus = xQueuePeek(xQueue, &infor, 0);

    if (xStatus == pdPASS){
      if (infor.targetID == 1){
        xQueueReceive(xQueue, &infor, 0);

        Serial.printf("Task 1 is handled, Value = %d\n", infor.dataValue);
      }
    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}


void TaskFunc2(void *pvParameter){

  Data_t infor;

  while(1){

    BaseType_t xStatus = xQueuePeek(xQueue, &infor, 0);

    if (xStatus == pdPASS){
      if (infor.targetID == 2){
        xQueueReceive(xQueue, &infor, 0);

        Serial.printf("Task 2 is handled, Value = %d\n", infor.dataValue);
      }
    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void TaskErrorHandle(void *pvParameter){

  Data_t infor;

  while(1){

    if (xQueueReceive(xQueue, &infor, 0) == pdPASS){

      Serial.printf("Error: Unknown request (ID = %d) ignored and removed!\n", infor.targetID);

    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  vTaskDelay(pdMS_TO_TICKS(1000));

  Serial.println("System Ready! Type [ID] [Value]");

  xQueue = xQueueCreate(10, sizeof(Data_t));

  if (xQueue != NULL){
    xTaskCreate(TaskReception, "Controller", 2048, NULL, 3, NULL);
    xTaskCreate(TaskFunc1, "Task1", 2048, NULL, 2, NULL);
    xTaskCreate(TaskFunc2, "Task2", 2048, NULL, 2, NULL);
    xTaskCreate(TaskErrorHandle, "Error_Handle", 2048, NULL, 1, NULL);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelete(NULL);
}