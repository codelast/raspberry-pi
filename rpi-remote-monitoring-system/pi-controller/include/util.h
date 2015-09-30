#ifndef	__UTIL_H
#define __UTIL_H

#include <string>

/**
 * This class implements some useful functionalities.
 *
 * @author Darran Zhang @ codelast.com
 */

class CUtil
{
 public:
  CUtil();
  virtual ~CUtil();

 private:
  time_t static getFileModTime(const std::string &filePath);
  
 public:
  void signalIgnore(int signalNumber);
  void static executeShellCommand(std::string shellCommand);
  std::string static getLatestFile(const std::string &dir);
  long long static getMillisecond();
  std::string static generateValidateStr(unsigned int hashWorkFactor,
				         int timestamp,
				         std::string token);
};

#endif
