#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * A program to control some LEDs through an pyroelectric module.
 *
 * @author Darran Zhang @ codelast.com
 */

int main (int argc,char* argv[])
{
  if (argc < 4) {
    printf("Usage example: ./sense-light pyroelectric_module_gpio_port led_one_gpio_port led_two_gpio_port\n");
    return 1;
  }
  int pyroelectricModuleGpioPort = atoi(argv[1]);
  int ledOneGpioPort = atoi(argv[2]);
  int ledTwoGpioPort = atoi(argv[3]);

  wiringPiSetup();

  pinMode(pyroelectricModuleGpioPort, INPUT);  // set mode to input
  pinMode(ledOneGpioPort, OUTPUT);   // set mode to output
  pinMode(ledTwoGpioPort, OUTPUT);

  int level = 0;
  while(1) {
    int currentLevel = digitalRead(pyroelectricModuleGpioPort);
    if (currentLevel != level) {
      printf("Current level: %d\n", currentLevel);
      digitalWrite(ledOneGpioPort, currentLevel);
      digitalWrite(ledTwoGpioPort, currentLevel);
      level = currentLevel;
    }
    delay(10);
  }

  return 0;
}
