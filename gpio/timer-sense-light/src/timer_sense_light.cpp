#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>  // std::fill_n()
#include <stdlib.h>   // atoi()
#include <sys/time.h>
#include <wiringPi.h>
#include <glog/logging.h>
#include "constants.h"
#include "util.h"

/**
 * A time-controllable sense light based on Raspberry Pi.
 * Please refer to the README.md for details.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

int getPositionInTimeRange(const string &hourAndMinute);
bool loadTimeRange(const string &timeRangeFile, int *timeRageArray);

int timeRangeArray[ONE_DAY_MINUTES];  // used to represents the status(enable/disable) of each minute of a day

int main (int argc,char* argv[])
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

  fill_n(timeRangeArray, ONE_DAY_MINUTES, DISABLE_STATUS);  // set all minutes of a day to disable status
  if (!loadTimeRange(timeRangeFile, timeRangeArray)) {  //read the time range file
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
    int currentTimePosition = getPositionInTimeRange(CUtil::getCurrentTime());
    if (DISABLE_STATUS == timeRangeArray[currentTimePosition]) {
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

/**
 * Get the position of a "hour:minute" string(e.g. "21:05") in the time range array.
 * The time ranger array is a 1440 elements array, which represent all the minutes of a day,
 * this function will calculate the position of a time in the array, e.g. if the time is 
 * "00:05", then the position is 6.
 *
 * @param hourAndMinute  The "hour:minute" string(e.g. "21:05").
 * @return the position value, -1 for something wrong.
 */
int getPositionInTimeRange(const string &hourAndMinute) {
  vector<string> items;  // each item e.g. "21"
  CUtil::stringSplit(hourAndMinute, ':', items);
  if (items.size() != 2) {
    LOG(ERROR) << "Invalid hour & minute: [" << hourAndMinute << "], current line will be skipped";
    return INVALID_POSITION;
  }
  int hour = atoi(items[0].c_str());
  int minute = atoi(items[1].c_str());
  return 60 * hour + minute;
}

/**
 * Load time range data from a file.
 * The format of the time range definition file looks like:
 * 21:00[\t]23:59
 * 00:00[\t]07:00
 *
 * This two lines means that: the LEDs can only be lighten up between 21:00~23:59 & 00:00~07:00,
 * at any other time of a day they will be turned off, even a person gets close to the pyroelectric 
 * module connected to RPi.
 *
 * @param timeRangeFile   A file stores the time range data.
 * @param timeRangeArray  The returned time range data pointer.
 * @return true for successfully loaded the data, false otherwise.
 */
bool loadTimeRange(const string &timeRangeFile, int *timeRangeArray) {
  ifstream ifs(timeRangeFile.c_str(), ios::in);
  if (!ifs.is_open()) {
    LOG(ERROR) << "Failed to open file [" << timeRangeFile << "]";
    return false;
  }
  string line;
  while (!ifs.eof()) {
    getline(ifs, line);
    if (line.empty()) {
      continue;
    }

    if (0 == line.compare(0, 1, "#")) {  // line starts with "#" indicates it's a comment line
      LOG(INFO) << "Read a comment line, skip";
      continue;
    }
    
    vector<string> lineItems;  // each item e.g. "21:00"
    CUtil::stringSplit(line, '\t', lineItems);
    if (lineItems.size() != 2) {
      LOG(WARNING) << "Skip invalid line: [" << line << "]";
      continue;
    }

    int startPosition = getPositionInTimeRange(lineItems[0]);
    if (INVALID_POSITION == startPosition) {
      continue;
    }
    int endPosition = getPositionInTimeRange(lineItems[1]);
    if (INVALID_POSITION == endPosition) {
      continue;
    }

    LOG(INFO) << "LED can be lighten up between [" << lineItems[0] << "~" << lineItems[1] << "]";

    /* set all minutes between the start & end position to enable status */
    for (int i = startPosition; i <= endPosition; i++) {
      timeRangeArray[i] = ENABLE_STATUS;
    }
  }
  ifs.close();

  return true;
}
