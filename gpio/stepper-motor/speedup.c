#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * A program to control a stepper motor(speedup) through the GPIO ports on Raspberry Pi.
 *
 * @author Darran Zhang @ codelast.com
 */

#define CLOCKWISE 1
#define COUNTER_CLOCKWISE 2
#define MAX_DELAY 15
#define MIN_DELAY 2

void rotate(int* pins, int direction, int delayMS);
void stop(int* pins);

int main (int argc,char* argv[])
{
  if (argc < 4) {
    printf("Usage example: ./motor direction gpio_port_1 gpio_port_2 gpio_port_3 gpio_port_4\n");
    return 1;
  }
  int direction = atoi(argv[1]);
  int gpioPort1 = atoi(argv[2]);
  int gpioPort2 = atoi(argv[3]);
  int gpioPort3 = atoi(argv[4]);
  int gpioPort4 = atoi(argv[5]);

  int pins[4] = {gpioPort1, gpioPort2, gpioPort3, gpioPort4};

  printf("Stepper motor connected to GPIO port [%d], [%d], [%d], [%d]\n",
	 gpioPort1, gpioPort2, gpioPort3, gpioPort4);

  wiringPiSetup();

  /* set mode to output */
  pinMode(gpioPort1, OUTPUT);
  pinMode(gpioPort2, OUTPUT);
  pinMode(gpioPort3, OUTPUT);
  pinMode(gpioPort4, OUTPUT);

  int delayMS = MAX_DELAY;
  while (1) {
    int i;
    for (i = 0; i < 10; i++) {
      rotate(pins, direction, delayMS);
    }
    --delayMS;
    if (delayMS < MIN_DELAY) {
      delayMS = MAX_DELAY;
      stop(pins);
      delay(1000);
    }
  }

  return 0;
}

/** 
 * Rotate the stepper motor.
 * @param pins       A pointer which points to the pins number array.
 * @param direction  CLOCKWISE for clockwise rotation, COUNTER_CLOCKWISE for counter clockwise rotation.
 */
void rotate(int* pins, int direction, int delayMS) {
  int i = 0;
  int j = 0;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      int pinIndex = (CLOCKWISE == direction) ? (3 -j) : j;
      if (j == i) {
	digitalWrite(pins[pinIndex], 1); // output a high level
      } else {
	digitalWrite(pins[pinIndex], 0); // output a low level
      }
      delay(delayMS);
    }
  }
}

/** 
 * Stop the stepper motor by setting all of its inputs to low level.
 * @param pins       A pointer which points to the pins number array.
 */
void stop(int* pins) {
  int i;
  for (i = 0; i < 4; i++) {
    digitalWrite(pins[i], 0);  // output a low level 
  }
}
