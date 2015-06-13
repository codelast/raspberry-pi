#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

/**
 * A program to turn on/off a LED through a button(the interrupt way, with software switch debouncing).
 *
 * @author Darran Zhang @ codelast.com
 */

typedef enum { false, true } bool;
#define INVALID_GPIO_PORT  -1
#define	DEBOUNCE_TIME      500

int level = 0;
int ledGpioPort = INVALID_GPIO_PORT;

long long recordMillisecond;

void interrupt2DoSth(void);
long long getMillisecond();

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

  recordMillisecond = getMillisecond();

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

  /* in a time period, we only allow the signal to be triggered once */
  long long currentMillisecond = getMillisecond();
  if (currentMillisecond - recordMillisecond < DEBOUNCE_TIME) {
    printf("Debouncing triggered @ %lld\n", currentMillisecond);
    return;
  }

  if (0 == level) {
    level = 1;
  } else {
    level = 0;
  }
  digitalWrite(ledGpioPort, level);  // turn on/off the LED

  // update the record time, to prepared for the next round debouncing
  recordMillisecond = currentMillisecond;
}

/** 
 * Get the milliseconds elapsed since Jan. 1, 1970.
 */
long long getMillisecond() {
  struct timeval tv;
  gettimeofday(&tv, NULL);

  return (long long) tv.tv_sec * 1000 + (long long) tv.tv_usec / 1000;
}
