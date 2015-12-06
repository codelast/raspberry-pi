#include <signal.h>     // signal related
#include <string.h>     // memset
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>     // atoi()
#include <glog/logging.h>
#include "constants.h"
#include "util.h"

/**
 * This class implements some useful functionalities.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

CUtil::CUtil() {
}

CUtil::~CUtil() {
}

/**
 * Ignore a signal.
 *
 * @param signalNumber	The signal identifier.
 */
void CUtil::signalIgnore(int signalNumber) {
  struct sigaction sa_SIG;
  memset(&sa_SIG, 0, sizeof(sa_SIG));
  sa_SIG.sa_handler = SIG_IGN;  // specify the signal handler function
  sigemptyset(&sa_SIG.sa_mask);
  sa_SIG.sa_flags = 0;
  sigaction(signalNumber, &sa_SIG, NULL);  // register
}

/**
 * Get the path of current program.
 *
 * @param buffer  Returned path be placed in.
 * @param len     Returned path length.
 * @return The number of characters in the path, -1 on error.
 */
size_t CUtil::getExecutablePath(char* buffer, size_t len) {
  char* pathEnd = NULL;

  if (readlink("/proc/self/exe", buffer, len) <= 0) {
    return -1;
  }

  pathEnd = strrchr(buffer, '/');
  if (NULL == pathEnd) {
    return -1;
  }

  ++pathEnd;
  *pathEnd = '\0';

  return (size_t)(pathEnd - buffer);
}

/**
 * Check whether a directory exists.
 *
 * @param path	The directory path.
 * @return true for the input dir exists, false otherwise.
 */
bool CUtil::isDirExist(string path) {
  if (path.empty()) {
    return false;
  }
  if (access(path.c_str(), F_OK) == 0) {
    return true;
  }
  return false;
}

/**
 * Get current time with the format of "HH:MM"(e.g. "21:05")
 *
 * @return current time string.
 */
string CUtil::getCurrentTime() {
  struct timeval tv;
  memset(&tv, 0, sizeof(tv));
  char timeStr[10] = "0";

  /* obtain the time of day, and convert it to a tm struct */
  gettimeofday(&tv, NULL);
  struct tm* ptm = localtime(&tv.tv_sec);

  strftime(timeStr, sizeof(timeStr), "%H:%M", ptm);  // format time
  return timeStr;
}

/**
 * Split a string by the specified delimiter.
 *
 * @param src        The string to split.
 * @param delimiter  The delimiter used to split the string.
 * @param output     The output vector which contains the split result.
 */
void CUtil::stringSplit(const string &src,
			char delimiter,
			vector<string> &output) {
  output.clear();
  if (src.empty()) {
    return;
  }
  
  string::size_type begin = 0, end = 0, length = src.length();
  while(begin < length && end != string::npos) {
    end = src.find(delimiter, begin);
    output.push_back(src.substr(begin, end - begin));
    begin = end + 1;
  }
  return;
}

/**
 * Get the position of a "hour:minute" string(e.g. "21:05") in the time range array.
 * The time ranger array is a 1440 elements array, which represent all the minutes of a day,
 * this function will calculate the position of a time in the array, e.g. if the time is 
 * "00:05", then the position is 5.
 *
 * @param hourAndMinute  The "hour:minute" string(e.g. "21:05").
 * @return the position value(zero-based), -1 for something wrong.
 */
int CUtil::getPositionInTimeRange(const string &hourAndMinute) {
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
 * Get the position of current time in the time range array.
 *
 * @return the position value, -1 for something wrong.
 */
int CUtil::getCurrentTimePositionInTimeRange() {
  return getPositionInTimeRange(getCurrentTime());
}
