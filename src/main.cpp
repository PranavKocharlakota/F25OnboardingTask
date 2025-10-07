#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Arduino.h>



// put function declarations here:
int myFunction(int, int);




void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

void vLowPriorityTask(void *pvParameters){
  for(;;){
    Serial.println("Low Priority!");
  }
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
void vHighPriorityTask(void *pvParameters){
  for(;;){
    printf("High Priority!");
  }
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  xTaskCreate(vLowPriorityTask, "Low", 1000, NULL, 1, NULL);
  xTaskCreate(vHighPriorityTask, "High", 1000, NULL, 2, NULL);
  vTaskStartScheduler();
}