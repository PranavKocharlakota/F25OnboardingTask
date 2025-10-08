#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Arduino.h>
#include <queue.h>

// Global Variables:
QueueHandle_t dataQueue;
float sensorBuffer[5];
int bufferIndex = 0;
int bufferCount = 0;

struct dataPack{
  float value;
  int error;
};

// Function definitions:

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