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

int scaleSuperMario[23] = {659, 659, 0, 659, 0, 523, 659, 0, 784, 0, 0, 0, 392, 0, 0, 0, 523, 0, 0, 392, 0, 0, 330};

int main (int argc, char *argv[]) {
  const char* programName = argv[0];
  if(argc < 2) {
    fprintf(stderr, "Usage: %s gpio_port\n", programName);
    return -1;
  }

  int gpioPort = atoi(argv[1]);

  if (-1 == wiringPiSetup()) {  // initialize WiringPi
    printf("Setup wiringPi failed!\n");
    return 1;
  }

  softToneCreate(gpioPort);
  int i;
  int loopCount = 0;
  int arrayLength = sizeof(scaleSuperMario) / sizeof(int);
  while (true) {
    printf("Has played the music for %d times\n", loopCount);
    
    for (i = 0; i < arrayLength; ++i) {
      softToneWrite(gpioPort, scaleSuperMario[i]);
      delay(200);
    }

    delay(500);
    ++loopCount;
  }
}
