#include <fstream>
#include <sstream>      // std::stringstream
#include <iomanip>      // std::setfill, std::setw
#include <linux/limits.h>  // PATH_MAX
#include <algorithm>    // std::fill_n()
#include <glog/logging.h>
#include <libconfig.h++>
#include "util.h"
#include "config_loader.h"

/**
 * This class stores the configuration of this program.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;
using namespace libconfig;

CConfigLoader::CConfigLoader() {
  fill_n(timeRangeArray, ONE_DAY_MINUTES, DISABLE_STATUS);  // set all minutes of a day to disable status
  pyroelectricGpioPort = 0;
  ledGpioPortStart = 1;
  ledNumber = 1;

  /* get current running program path */
  char path[PATH_MAX];
  memset(path, 0, sizeof(path));
  if (-1 != (int) CUtil::getExecutablePath(path, sizeof(path))) {
    currentAppPath = path;
  }

  pthread_rwlock_init(&timeRangeDataLock, NULL);
  manualMode = false;
  ledLevel = 0;
  threadRunning = true;
}

CConfigLoader::~CConfigLoader() {
  pthread_rwlock_destroy(&timeRangeDataLock);
}

/**
 * Load main configuration from a file.
 *
 * @return true for successfully loaded the config file, false otherwise.
 */
bool CConfigLoader::loadMainConfig(const string configFile) {
  Config cfg;

  try {
    cfg.readFile(configFile.c_str());
  } catch(const FileIOException &fioex) {    
    LOG(ERROR) << "I/O exception when loading config file.";
    return false;
  } catch(const ParseException &pex) {
    LOG(ERROR) << "Parse config file error at " << pex.getFile() << " : " << pex.getLine() << " - " << pex.getError();
    return false;
  }

  // get the root object of the config object 
  const Setting& root = cfg.getRoot();

  try {
    const Setting &gpio = root[CFG_FILE_SECTION_GPIO.c_str()];

    if (!(gpio.lookupValue("PyroelectricGpioPort", pyroelectricGpioPort) &&
	  gpio.lookupValue("LedGpioPortStart", ledGpioPortStart) &&
	  gpio.lookupValue("LedNumber", ledNumber))) {
      LOG(ERROR) << "Failed to read config file section " << CFG_FILE_SECTION_GPIO;
    }
  } catch (...) {
    LOG(ERROR) << "Exception caught when reading config file section " << CFG_FILE_SECTION_GPIO;
    return false;
  }

  try {
    const Setting &web = root[CFG_FILE_SECTION_WEB.c_str()];

    if (!(web.lookupValue("ListenPort", listenPort) &&
	  web.lookupValue("WebRootDirName", webRootDirName))) {
      LOG(ERROR) << "Failed to read config file section " << CFG_FILE_SECTION_WEB;
    }
  } catch (...) {
    LOG(ERROR) << "Exception caught when reading config file section " << CFG_FILE_SECTION_WEB;
    return false;
  }

  webRootPath = currentAppPath + "/" + webRootDirName;
  return true;
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
bool CConfigLoader::loadTimeRangeFromFile(const string &timeRangeFile) {
  ifstream ifs(timeRangeFile.c_str(), ios::in);
  if (!ifs.is_open()) {
    LOG(ERROR) << "Failed to open file [" << timeRangeFile << "]";
    return false;
  }
  
  string line;
  vector<string> lines;
  while (!ifs.eof()) {
    getline(ifs, line);
    if (line.empty()) {
      continue;
    }
    lines.push_back(line);
  }
  loadTimeRange(lines);

  ifs.close();
  return true;
}

/**
 * Load time range data from strings like the lines in the time range definition file.
 *
 * @param timeRangeLines  Each item contains a string like one line in the time range def file.
 * @return true for successfully loaded the data, false otherwise.
 */
bool CConfigLoader::loadTimeRange(const vector<string> &timeRangeLines) {
  if (timeRangeLines.empty()) {
    return false;
  }

  pthread_rwlock_wrlock(&timeRangeDataLock);

  fill_n(timeRangeArray, ONE_DAY_MINUTES, DISABLE_STATUS);  // set all minutes of a day to disable status
  
  vector<string>::const_iterator it;
  for (it = timeRangeLines.begin(); it != timeRangeLines.end(); it++) {
    string line = *it;
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

  pthread_rwlock_unlock(&timeRangeDataLock);
  return true;
}

/**
 * Load time range data from a string.
 *
 * @param timeRangeLines  A string contains the time range data, 
 *                        e.g. "00:05\t09:00\n16:00\t23:00"
 * @return true for successfully loaded the data, false otherwise.
 */
bool CConfigLoader::loadTimeRange(const string &timeRangeLines) {
  if (timeRangeLines.empty()) {
    return false;
  }

  vector<string> vec;
  CUtil::stringSplit(timeRangeLines, '\n', vec);
  return loadTimeRange(vec);
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

/**
 * Convert a position index to a time string.
 *
 * @param position  A position index([0, 1023]) in the time range array.
 * @return A time string, e.g. "21:00".
 */
string CConfigLoader::convertPosition2TimeStr(int position) {
  int hour = position / 60;
  int minute = position % 60;

  stringstream ss;
  // if the hour or minute is one digit(e.g. 5), add a "0" before it(e.g. "05")
  ss << setfill('0') << setw(2) << hour << ":" << setfill('0') << setw(2) << minute;
  return ss.str();
}

/**
 * Translate the time range array data to literal time range string(e.g. "22:00~23:59").
 *
 * @param timeRangeArray  An int array which stores the time range data, each item is 0 or 1.
 * @param output  The output vectore which stores all the time range strings.
 */
void CConfigLoader::translateTimeRange2String(vector<string> &output) {
  pthread_rwlock_rdlock(&timeRangeDataLock);
  
  string startTimeStr;  // e.g. "21:00"
  for (int i = 0; i < ONE_DAY_MINUTES; i++) {
    if (ENABLE_STATUS == timeRangeArray[i]) {
      string timeStr = convertPosition2TimeStr(i);
      if (!startTimeStr.empty()) {  // already recorded the start time
	if ((i == ONE_DAY_MINUTES - 1) ||
	    ((i < (ONE_DAY_MINUTES - 1) && DISABLE_STATUS == timeRangeArray[i + 1]))) {
	  output.push_back(startTimeStr + " ~ " + timeStr);
	  startTimeStr.clear();
	}
      } else {
	startTimeStr = timeStr;
      }
    }
  }

  pthread_rwlock_unlock(&timeRangeDataLock);
} 
