#ifndef	__CONFIG_LOADER_H
#define __CONFIG_LOADER_H

#include <string>
#include <vector>
#include <pthread.h>  // pthread_rwlock_t
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

  std::string currentAppPath;  // the path of the current running program
  int pyroelectricGpioPort;
  int ledGpioPortStart;
  int ledNumber;

  int listenPort;       // local web sever listen port
  std::string webRootDirName;
  char webRootPath[1024];

  pthread_rwlock_t timeRangeDataLock;
  bool manualMode;     // whether the LEDs are under manual control mode
  int ledLevel;        // used to control the LED status(on/off)
  bool threadRunning;  // a flag to control the thread running status
  std::string timeRangeFile;  // a file on local fs which stores the time range definition data

 private:
  std::string convertPosition2TimeStr(int position);
  bool loadTimeRange(const std::vector<std::string> &timeRangeLines);

 public:
  bool loadMainConfig(const std::string mainConfigFile);
  bool loadTimeRangeFromFile(const std::string &timeRangeFile);
  bool updateTimeRange(const std::string &timeRangeLines);
  int getTimePositionStatus(int timePosition);
  void translateTimeRange2String(std::vector<std::string> &output, std::string delimiter);

  inline int getPyroelectricGpioPort() const { return pyroelectricGpioPort; }
  inline int getLedGpioPortStart() const { return ledGpioPortStart; }
  inline int getLedNumber() const { return ledNumber; }
  inline int getListenPort() const { return listenPort; }
  inline const char* getWebRootPath() const { return webRootPath; }
  inline bool isManualMode() const { return manualMode; }
  inline void setManualMode(bool val) { manualMode = val; }
  inline int getLedLevel() const { return ledLevel; }
  inline void setLedLevel(int val) { ledLevel = val; }
  inline bool getThreadRunning() const { return threadRunning; }
  inline void setThreadRunning(bool val) { threadRunning = val; }
};

#endif





