#include <iostream>
#include <signal.h>
#include <string.h>
#include <iostream>
#include <wiringPi.h>
#include <glog/logging.h>
#include "util.h"
#include "config.h"
#include "thread_control_logic.h"
#include "constants.h"
 
/**
 * The entrance of this program. 
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

// global variables
CConfig gConfig;

void signalCatchSIGINT(int signalNumber);

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);                // initialize Google's logging library 
  google::SetStderrLogging(google::INFO);            // set logging level 

  gConfig.setConfigFile(argv[1]);
  if (!gConfig.loadConfig()) {
    LOG(ERROR) << "Failed to load pi-controller config file, please check its content";
    return 1;
  }

  // don't need to check the return value of wiringPiSetup series functions because WiringPi Version 2 always return 0(refer to http://wiringpi.com/reference/setup/)
  wiringPiSetup();

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

  /* create the command execution thread */
  pthread_t cmdExecThreadId = 0;
  int createThreadRet = pthread_create(&cmdExecThreadId, NULL, threadCmdExec, NULL);
  if (0 != createThreadRet) {
    LOG(ERROR) << "Failed to create the command execution thread, quit";
    return 1;
  }

  LOG(INFO) << "pi-controller initialization done";

  pthread_join(cmdExecThreadId, NULL);	// wait thread to finish 

  LOG(INFO) << "pi-controller exit";
  return 0;
}

/**
 * Signal SIGINT handler.
 *
 * @param signalNumber	Signal identifier.
 */
void signalCatchSIGINT(int signalNumber) {
  char msg[] = "Received SIGINT, pi-controller will exit...\n";
  write(STDERR_FILENO, msg, strlen(msg));    
  gConfig.setThreadRunning(false);
}
