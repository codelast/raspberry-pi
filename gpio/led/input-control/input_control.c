#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * A program to control a LED through a button using WiringPi.
 *
 * @author Darran Zhang @ codelast.com
 */

int main (int argc,char* argv[])
{
  if (argc < 3) {
    printf("Usage example: ./input button_gpio_port led_gpio_port\n");
    return 1;
  }
  int buttonGpioPort = atoi(argv[1]);
  int ledGpioPort = atoi(argv[2]);

  if (-1 == wiringPiSetup()) {
    printf("Setup wiringPi failed!\n");
    return 1;
  }

  pinMode(buttonGpioPort, INPUT);  // set mode to input
  pinMode(ledGpioPort, OUTPUT);  // set mode to output
  //pullUpDnControl(buttonGpioPort, PUD_UP);

  while(1) {
    int currentLevel = digitalRead(buttonGpioPort);
    if (1 == digitalRead(buttonGpioPort)) {
      digitalWrite(ledGpioPort, 0);
    } else {
      digitalWrite(ledGpioPort, 1);
    }
    delay(10);
  }

  return 0;
}
