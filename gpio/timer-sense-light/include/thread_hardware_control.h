#ifndef	__THREAD_HARDWARE_CONTROL_H
#define __THREAD_HARDWARE_CONTROL_H

#include <glog/logging.h>
#include <wiringPi.h>
#include "constants.h"
#include "util.h"
#include "config_loader.h"

/**
 * This thread implements the main hardware control logic.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

extern CConfigLoader gConfigLoader;

/**
 * Turn on/off all the LEDs.
 *
 * @param ledGpioPortStart  The GPIO port of the 1st LED.
 * @param ledNumber         The total number of LEDs.
 * @param level             The level to set to the GPIO port.
 */
void turnOnOffAllLED(int ledGpioPortStart, int ledNumber, int level) {
  for (int i = 0; i < ledNumber; i++) {
    digitalWrite(ledGpioPortStart + i, level);
  }
}

void* threadHardwareControl(void*) {
  // don't need to check the return value of wiringPiSetup series functions because WiringPi Version 2 always return 0(refer to http://wiringpi.com/reference/setup/)
  wiringPiSetup();

  int pyroelectricGpioPort = gConfigLoader.getPyroelectricGpioPort();
  int ledGpioPortStart = gConfigLoader.getLedGpioPortStart();
  int ledNumber = gConfigLoader.getLedNumber();

  /* set GPIO ports mode */
  pinMode(pyroelectricGpioPort, INPUT);  // set mode to input
  for (int i = 0; i < ledNumber; i++) {
    pinMode(ledGpioPortStart + i, OUTPUT);   // set all LED GPIO port mode to output
  }
  
  // turn off all the LEDs at the initial status
  turnOnOffAllLED(ledGpioPortStart, ledNumber, LOW);

  LOG(INFO) << "Hardware controlling thread starts to work...";

  int level = 0;
  while(gConfigLoader.getThreadRunning()) {
    /* manual control mode has higher priority */
    if (gConfigLoader.isManualMode()) {
      turnOnOffAllLED(ledGpioPortStart, ledNumber, gConfigLoader.getLedLevel());
      delay(10);
      continue;
    }

    int currentTimePosition = CUtil::getCurrentTimePositionInTimeRange();
    if (DISABLE_STATUS == gConfigLoader.getTimePositionStatus(currentTimePosition)) {
      /* turn off all the LEDs */
      for (int i = 0; i < ledNumber; i++) {
	digitalWrite(ledGpioPortStart + i, LOW);
      }
    } else {
      int currentLevel = digitalRead(pyroelectricGpioPort);
      if (currentLevel != level) {
	LOG(INFO) << "Level change detected, current level is: " << currentLevel;

	/* turn on/off all LEDs according to current input level */
	for (int i = 0; i < ledNumber; i++) {
	  digitalWrite(ledGpioPortStart + i, currentLevel);
	}
	level = currentLevel;
      }
    }
    delay(10);
  }

  // turn off all the LEDs when thread exits
  turnOnOffAllLED(ledGpioPortStart, ledNumber, LOW);

  return NULL;
}

#endif
