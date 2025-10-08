#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Arduino.h>
#include <queue.h>


//Function declarations: 


//Global Variables:
QueueHandle_t testQueue;
float sensorBuffer[5];
int bufferIndex = 0;
int bufferCount = 0;

struct dataPack{
  float value;
  int error;
};

//Function definitions:

float generateSensorSignal(){
  return random(-10, 111); 
}

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

void vSensorTask(void *pvParameters){
  for(;;){
    float sensorValue = generateSensorSignal();
    float filteredValue = movingAverage(sensorValue);
    struct dataPack d1;
    d1.value = filteredValue;
    if (filteredValue < 0 || filteredValue > 100){
      d1.error = 1;
    }
    xQueueSend(testQueue, &d1, portMAX_DELAY);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void vCanTask(void *pvParameters){
  dataPack d1;
  for(;;){
    xQueueReceive(testQueue, &d1, portMAX_DELAY);
    char buffer[50];
    sprintf(buffer, "CAN[ID=0x123]: %.0f%%, %s", d1.value, d1.error ? "ERROR" : "OK");
    Serial.println(buffer);
  }
}

void setup() {
  Serial.begin(115200);
  testQueue = xQueueCreate(5, sizeof(struct dataPack));
  xTaskCreate(vSensorTask, "Sensor", 1000, NULL, 1, NULL);
  xTaskCreate(vCanTask, "Can", 1000, NULL, 2, NULL);
  vTaskStartScheduler();
}

void loop(){

}