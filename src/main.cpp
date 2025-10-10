#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Arduino.h>
#include <queue.h>
#include <globals.h>
#include <can.h>
#include <sensor.h>


// Function definitions:
void setup() {
  // Initialize Serial at 115200 baud rate.
  Serial.begin(115200);
  // Creates a queue with space for 5 dataPack structures.
  dataQueue = xQueueCreate(5, sizeof(struct dataPack));
  // Creates the Sensor task with priority 1(lower).
  xTaskCreate(vSensorTask, "Sensor", 1000, NULL, 1, NULL);
  // Creates the Can task with priority 2(higher).
  xTaskCreate(vCanTask, "Can", 1000, NULL, 2, NULL);
  // Function will start running tasks according to priority and blocks. 
  vTaskStartScheduler();
}


void loop(){

}