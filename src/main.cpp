#include <stdio.h>
//#include <FreeRTOS.h>

// put function declarations here:
int myFunction(int, int);


int main() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  printf("%d", result);
  return 0;
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

