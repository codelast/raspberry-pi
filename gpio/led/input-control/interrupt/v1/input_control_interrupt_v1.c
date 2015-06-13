#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * A program to turn on/off a LED through a button(the interrupt way, with no software switch debouncing).
 *
 * @author Darran Zhang @ codelast.com
 */

typedef enum { false, true } bool;
#define INVALID_GPIO_PORT  -1

int level = 0;
int ledGpioPort = INVALID_GPIO_PORT;

void interrupt2DoSth(void);

int main (int argc,char* argv[])
{
  if (argc < 3) {
    printf("Usage example: ./%s button_gpio_port led_gpio_port\n", argv[0]);
    return 1;
  }
  int buttonGpioPort = atoi(argv[1]);
  ledGpioPort = atoi(argv[2]);

  wiringPiSetup();

  // register the function to deal with the button events
  wiringPiISR(buttonGpioPort, INT_EDGE_FALLING, &interrupt2DoSth);
  
  // set mode to output, which will be used to turn on/off the LED
  pinMode(ledGpioPort, OUTPUT);

  int currentLevel = 0;
  while(true) {
    if (currentLevel != level) {
      printf("Caught an interrupt\n");
      currentLevel = level;
    }
  }

  return 0;
}

/** 
 * The function to deal with the interrupt signal triggered by a button. 
 */
void interrupt2DoSth(void) {
  if (INVALID_GPIO_PORT == ledGpioPort) {
    printf("LED GPIO port was not set properly\n");
    return;
  }

  if (0 == level) {
    level = 1;
  } else {
    level = 0;
  }
  digitalWrite(ledGpioPort, level);  // turn on/off the LED
}
