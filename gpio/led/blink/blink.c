#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * A program to blink the LED using WiringPi.
 *
 * @author Darran Zhang @ codelast.com
 */

int main (int argc,char* argv[])
{
  if (argc < 2) {
    printf("Usage example: ./blink gpio_port\n");
    return 1;
  }
  int gpioPort = atoi(argv[1]);

  wiringPiSetup();

  pinMode(gpioPort, OUTPUT);  // set mode to output

  int level = 0;
  while(1) {
    level = (0 == level) ? 1 : 0;
    digitalWrite(gpioPort, level); // output a low/high level to the specified GPIO port
    sleep(1);
  }

  return 0;
}
