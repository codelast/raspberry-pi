#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * A program to turn on/off a LED through a button(the polling way).
 *
 * @author Darran Zhang @ codelast.com
 */

int main (int argc,char* argv[])
{
  if (argc < 3) {
    printf("Usage example: ./%s button_gpio_port led_gpio_port\n", argv[0]);
    return 1;
  }
  int buttonGpioPort = atoi(argv[1]);
  int ledGpioPort = atoi(argv[2]);

  wiringPiSetup();

  pinMode(buttonGpioPort, INPUT);  // set mode to input
  pinMode(ledGpioPort, OUTPUT);    // set mode to output

  int level = 0;
  while(1) {
    int currentLevel = digitalRead(buttonGpioPort);

    // when the button is pressed/released, we want the LED to be turn on/off
    if (level != currentLevel) {
      digitalWrite(ledGpioPort, level);  // turn on/off the LED
      level = currentLevel;
    }
    
    delay(10);
  }

  return 0;
}
