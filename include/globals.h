#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <FreeRTOS.h>
#include <queue.h>

struct dataPack{
  float value;
  int error;
};

extern QueueHandle_t dataQueue;

#endif
