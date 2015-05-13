#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * A program to control a stepper motor through the GPIO ports of Raspberry Pi.
 *
 * @author Darran Zhang @ codelast.com
 */

#define CLOCKWISE 1
#define COUNTER_CLOCKWISE 2

void rotate(int* pins, int direction, int delayMS);

int main (int argc,char* argv[])
{
  if (argc < 4) {
    printf("Usage example: ./motor gpio_port_1 gpio_port_2 gpio_port_3 gpio_port_4\n");
    return 1;
  }
  int gpioPort1 = atoi(argv[1]);
  int gpioPort2 = atoi(argv[2]);
  int gpioPort3 = atoi(argv[3]);
  int gpioPort4 = atoi(argv[4]);

  int pins[4] = {gpioPort1, gpioPort2, gpioPort3, gpioPort4};

  printf("Stepper motor connected to GPIO port [%d], [%d], [%d], [%d]\n",
	 gpioPort1, gpioPort2, gpioPort3, gpioPort4);

  if (-1 == wiringPiSetup()) {
    printf("Setup wiringPi failed!\n");
    return 1;
  }

  /* set mode to output */
  pinMode(gpioPort1, OUTPUT);
  pinMode(gpioPort2, OUTPUT);
  pinMode(gpioPort3, OUTPUT);
  pinMode(gpioPort4, OUTPUT);

  while (1) {
    rotate(pins, CLOCKWISE, 4);
  }

  return 0;
}

/* Rotate the motor.
 *  @param pins     A pointer which points to the pins number array.
 *  @param direction  CLOCKWISE for clockwise rotation, COUNTER_CLOCKWISE for counter clockwise rotation.
 */
void rotate(int* pins, int direction, int delayMS) {
  int i = 0;
  int j = 0;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (j == i) {
	if (CLOCKWISE == direction) {
	  digitalWrite(pins[3 - j], 1); // output a high level
	  delay(delayMS);
	} else {
	  digitalWrite(pins[j], 1);
	  delay(delayMS);
	}
      } else {
	if (CLOCKWISE == direction) {
	  digitalWrite(pins[3 - j], 0); // output a low level
	  delay(delayMS);
	} else {
	  digitalWrite(pins[j], 0);
	  delay(delayMS);
	}
      }
    }
  }
}
