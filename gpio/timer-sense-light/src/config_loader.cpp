#include <fstream>
#include <vector>
#include <algorithm>  // std::fill_n()
#include <glog/logging.h>
#include "util.h"
#include "config_loader.h"

/**
 * This class stores the configuration of this program.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

CConfigLoader::CConfigLoader() {
  fill_n(timeRangeArray, ONE_DAY_MINUTES, DISABLE_STATUS);  // set all minutes of a day to disable status
}

CConfigLoader::~CConfigLoader() {
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
bool CConfigLoader::loadTimeRange(const string &timeRangeFile) {
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

    int startPosition = CUtil::getPositionInTimeRange(lineItems[0]);
    if (INVALID_POSITION == startPosition) {
      continue;
    }
    int endPosition = CUtil::getPositionInTimeRange(lineItems[1]);
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

/**
 * Get the status(enable/disable) of a time position.
 *
 * @param timePosition  The time position.
 * @return the status(enable/disable) of a time position in the time range array, 
 *         -1 for providing an invalid time position.
 */
int CConfigLoader::getTimePositionStatus(int timePosition) {
  if (timePosition < 0 || timePosition >= ONE_DAY_MINUTES) {
    LOG(ERROR) << "Invalid time position: " << timePosition;
    return INVALID_POSITION;
  }

  return timeRangeArray[timePosition];
}
