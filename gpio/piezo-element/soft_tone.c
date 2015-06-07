#include <stdio.h>
#include <stdlib.h>

#include <wiringPi.h>
#include <softTone.h>

/**
 * A program to control Raspberry Pi to play music through a piezo element using WiringPi.
 *
 * @author Darran Zhang @ codelast.com
 */

typedef enum { false, true } bool;

int scale[23] = {659, 659, 0, 659, 0, 523, 659, 0, 784, 0, 0, 0, 392, 0, 0, 0, 523, 0, 0, 392, 0, 0, 330};

int main (int argc, char *argv[]) {
  const char* programName = argv[0];
  if(argc < 2) {
    fprintf(stderr, "Usage: %s gpio_port\n", programName);
    return -1;
  }

  int gpioPort = atoi(argv[1]);

  wiringPiSetup();

  if (0 != softToneCreate(gpioPort)) {
    printf("Create software tone failed!\n");
    return 1;
  }

  int i;
  int loopCount = 0;
  int arrayLength = sizeof(scale) / sizeof(int);
  while (true) {
    printf("Has played the music for %d times\n", loopCount);
    
    for (i = 0; i < arrayLength; ++i) {
      softToneWrite(gpioPort, scale[i]);
      delay(200);
    }

    delay(500);
    ++loopCount;
  }
}
