#include <string>
#include <stdlib.h>   // atoi()
#include <wiringPi.h>
#include <glog/logging.h>
#include "constants.h"
#include "config_loader.h"
#include "util.h"

/**
 * A time-controllable human sense light based on Raspberry Pi.
 * Please refer to the README.md for details.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

// global variables
CConfigLoader gConfigLoader;

int main(int argc,char* argv[])
{
  google::InitGoogleLogging(argv[0]);      // initialize Google's logging library 
  google::SetStderrLogging(google::INFO);  // set logging level 

  if (argc < 4) {
    LOG(ERROR) << "Usage example: ./timer_sense_light pyroelectric_module_gpio_port led_gpio_port_start led_number";
    return 1;
  }

  string timeRangeFile = argv[1];
  int pyroelectricModuleGpioPort = atoi(argv[2]);
  int ledGpioPortStart = atoi(argv[3]);
  int ledNumber = atoi(argv[4]);

  if (!gConfigLoader.loadTimeRange(timeRangeFile)) {  // read the time range file
    return 1;
  }
  
  wiringPiSetup();

  pinMode(pyroelectricModuleGpioPort, INPUT);  // set mode to input
  for (int i = 0; i < ledNumber; i++) {
    pinMode(ledGpioPortStart + i, OUTPUT);   // set all LED GPIO port mode to output
  }

  /* turn off all the LEDs at the initial status */
  for (int i = 0; i < ledNumber; i++) {
    digitalWrite(ledGpioPortStart + i, 0);
  }

  int level = 0;
  while(true) {
    int currentTimePosition = CUtil::getPositionInTimeRange(CUtil::getCurrentTime());
    if (DISABLE_STATUS == gConfigLoader.getTimePositionStatus(currentTimePosition)) {
      /* turn off all the LEDs */
      for (int i = 0; i < ledNumber; i++) {
	digitalWrite(ledGpioPortStart + i, 0);
      }
    } else {
      int currentLevel = digitalRead(pyroelectricModuleGpioPort);
      if (currentLevel != level) {
	LOG(INFO) << "Level change detected, current level is: " << currentLevel;

	/* turn on/off all LEDs according to current input level */
	for (int i = 0; i < ledNumber; i++) {
	  digitalWrite(ledGpioPortStart, currentLevel);
	}
	level = currentLevel;
      }
    }
    delay(10);
  }

  return 0;
}
