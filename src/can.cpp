#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Arduino.h>
#include <queue.h>
#include <globals.h>


// Receives data from dataQueue and prints it to Serial in CAN Frame Format.
void vCanTask(void *pvParameters){
  struct dataPack d1;
  for(;;){
    // Takes the dataPack d1 from dataQueue. Task waits indefinitely until queue has data allowing CPU to process other tasks.
    xQueueReceive(dataQueue, &d1, portMAX_DELAY);
    char buffer[50];
    sprintf(buffer, "CAN[ID=0x123]: %.0f%%, %s", d1.value, d1.error ? "ERROR" : "OK");
    // Prints to Serial Monitor
    Serial.println(buffer);
    // Adds a small delay to allow lower-priority tasks to run.
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}