#ifndef	__UTIL_H
#define __UTIL_H

#include <string>
#include <vector>

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

 public:
  std::string static getCurrentTime();
  void static stringSplit(const std::string &src,
			  char delimiter,
			  std::vector<std::string> &output);
};

#endif
