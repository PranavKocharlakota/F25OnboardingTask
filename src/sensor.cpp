#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Arduino.h>
#include <queue.h>
#include <globals.h>


// Global Variables:
float sensorBuffer[5];
int bufferIndex = 0;
int bufferCount = 0;

// Randomly generates a number between -10 and 111 and represents a sensor value like throttle %.
float generateSensorSignal(){
  return random(-10, 111); 
}

// Takes five or less sensor values and computes their average.
float movingAverage(float newValue){
  sensorBuffer[bufferIndex] = newValue;
  bufferIndex = (bufferIndex + 1) % 5;
  if (bufferCount < 5) {bufferCount++;}
  float sum = 0;
  for (int i = 0; i < bufferCount; i++){
    sum += sensorBuffer[i];
  }
  return sum/bufferCount;
}

// Takes filtered data and determines error. Sends data into queue.
void vSensorTask(void *pvParameters){
  for(;;){
    float sensorValue = generateSensorSignal();
    float filteredValue = movingAverage(sensorValue);
    struct dataPack d1;
    d1.value = filteredValue;
    d1.error = (filteredValue < 0 || filteredValue > 100) ? 1 : 0;
    // Adds the dataPack d1 to the testQueue. Task waits indefinitely until queue is emptied allowing CPU to process other tasks.
    xQueueSend(dataQueue, &d1, portMAX_DELAY);
    // Adds a small delay to allow lower-priority tasks to run.
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}