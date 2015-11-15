#include <string>
#include <glog/logging.h>
#include "config_loader.h"
#include "util.h"
#include "thread_web_handler.h"
#include "thread_hardware_control.h"

/**
 * A time-controllable human sense light based on Raspberry Pi.
 * Please refer to the README.md for details.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

// global variables
CConfigLoader gConfigLoader;

void signalCatchSIGINT(int signalNumber);

int main(int argc,char* argv[]) {
  google::InitGoogleLogging(argv[0]);      // initialize Google's logging library
  google::SetStderrLogging(google::INFO);  // set logging level

  if (argc < 3) {
    LOG(ERROR) << "Usage example: ./timer_sense_light main_config_file time_range_config_file";
    return 1;
  }

  if (!gConfigLoader.loadMainConfig(argv[1])) {
    LOG(ERROR) << "Failed to load main config file, please check";
    return 1;
  }

  string timeRangeFile = argv[2];
  if (!gConfigLoader.loadTimeRangeFromFile(timeRangeFile)) {  // load time range data from file
    return 1;
  }

  /* signal handler */
  CUtil::signalIgnore(SIGHUP);	// ignore SIGHUP signal, to ensure that current program can live after the terminal close 
  CUtil::signalIgnore(SIGUSR1);	// ignore SIGUSR1 
  CUtil::signalIgnore(SIGUSR2);	// ignore SIGUSR2 

  /* register SIGINT signal handler */
  struct sigaction saSIGINT;
  memset(&saSIGINT, 0, sizeof(saSIGINT));
  saSIGINT.sa_flags = 0;
  saSIGINT.sa_handler = &signalCatchSIGINT;  // specify the signal handler 
  sigaction(SIGINT, &saSIGINT, NULL);        // register 

  /* create the web events handling thread */
  pthread_t webHandlerThreadId = 0;
  int createThreadRet = pthread_create(&webHandlerThreadId, NULL, threadWebHandler, NULL);
  if (0 != createThreadRet) {
    LOG(ERROR) << "Failed to create the web events handling thread, quit";
    return 1;
  }

  /* create the hardrware control thread */
  pthread_t hardwareControlThreadId = 0;
  createThreadRet = pthread_create(&hardwareControlThreadId, NULL, threadHardwareControl, NULL);
  if (0 != createThreadRet) {
    LOG(ERROR) << "Failed to create the hardware control thread, quit";
    return 1;
  }

  /* wait thread to finish */
  pthread_join(webHandlerThreadId, NULL);
  pthread_join(hardwareControlThreadId, NULL);

  LOG(INFO) << "Backend program exits";
  return 0;
}

/**
 * Signal SIGINT handler.
 *
 * @param signalNumber  Signal identifier.
 */
void signalCatchSIGINT(int signalNumber) {
  LOG(INFO) << "Received SIGINT, will exit...";
  gConfigLoader.setThreadRunning(false);
}
