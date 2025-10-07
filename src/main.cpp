#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <Arduino.h>
#include <queue.h>


//Function declarations: 
int myFunction(int, int);

//Global Variables:
QueueHandle_t testQueue;
float sensorBuffer[5];
int bufferIndex = 0;
int bufferCount = 0;

struct dataPack{
  float value;
  int error;
};

void loop() {
  // put your main code here, to run repeatedly:
}

//Function definitions:
int myFunction(int x, int y) {
  return x + y;
}

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
  printf("%d" , sum/bufferCount);
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  testQueue = xQueueCreate(5, sizeof(struct dataPack));
  xTaskCreate(vSensorTask, "Sensor", 1000, NULL, 1, NULL);
  vTaskStartScheduler();

}