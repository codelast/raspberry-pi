#ifndef	__CONFIG_LOADER_H
#define __CONFIG_LOADER_H

#include <string>
#include "constants.h"

/**
 * This class stores the configuration of this program.
 *
 * @author Darran Zhang @ codelast.com
 */

class CConfigLoader
{
 public:
  CConfigLoader();
  virtual ~CConfigLoader();

 private:
  int timeRangeArray[ONE_DAY_MINUTES];  // used to represents the status(enable/disable) of each minute of a day

 public:
  bool loadTimeRange(const std::string &timeRangeFile);
  int getTimePositionStatus(int timePosition);
};

#endif
