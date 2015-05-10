#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * A program to control the brightness of LED through hardware PWM using WiringPi.
 *
 * @author Darran Zhang @ codelast.com
 */

const int MAX_PWM_VALUE = 1023;

int main (int argc,char* argv[])
{
  if (argc < 2) {
    printf("Usage example: ./pwm_hardware gpio_port\n");
    return 1;
  }
  int gpioPort = atoi(argv[1]);

  if (-1 == wiringPiSetup()) {
    printf("Setup wiringPi failed!");
    return 1;
  }

  pinMode(gpioPort, PWM_OUTPUT);  // set mode to PWM output

  int brightness = 0;
  while(1) {
    for (brightness = 0; brightness <= MAX_PWM_VALUE; ++brightness) {
      pwmWrite(gpioPort, brightness);  // set the PWM value for the specified PWM pin
      delay(1);  // pause N milliseconds
    }

    for (brightness = MAX_PWM_VALUE; brightness >= 0; --brightness) {
      pwmWrite(gpioPort, brightness);
      delay(1);
    }
  }

  return 0;
}
