#include <string.h>	// memset 
#include <time.h>
#include <sys/time.h>
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
 * Get current time with the format of "HH:MM"(e.g. "21:05")
 *
 * @param currentTime  The returned time string.
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
  string::size_type begin = 0, end = 0, length = src.length();
  while(begin < length && end != string::npos) {
    end = src.find(delimiter, begin);
    output.push_back(src.substr(begin, end-begin));
    begin = end + 1;
  }
  return;
}
