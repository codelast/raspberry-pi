#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

/**
 * A time-controllable sense light based on Raspberry Pi.
 * Please refer to the README.md for more details.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

int main (int argc,char* argv[])
{
  if (argc < 4) {
    printf("Usage example: ./sense_light pyroelectric_module_gpio_port led_gpio_port_start led_number\n");
    return 1;
  }
  string timeRangeFile = argv[1];
  int pyroelectricModuleGpioPort = atoi(argv[2]);
  int ledGpioPortStart = atoi(argv[3]);
  int ledNumber = atoi(argv[4]);

  //TODO: read the time range file
  
  wiringPiSetup();

  pinMode(pyroelectricModuleGpioPort, INPUT);  // set mode to input
  for (int i = 0; i < ledNumber; i++) {
    pinMode(ledGpioPortStart + i, OUTPUT);   // set all LED GPIO port mode to output
  }

  int level = 0;
  while(1) {
    int currentLevel = digitalRead(pyroelectricModuleGpioPort);
    if (currentLevel != level) {
      printf("Current level: %d\n", currentLevel);

      // turn on/off all LEDs
      for (int i = 0; i < ledNumber; i++) {
	digitalWrite(ledGpioPortStart, currentLevel);
      }
    }
    delay(10);
  }

  return 0;
}
