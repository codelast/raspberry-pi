#include <stdio.h>
#include <signal.h>	// signal related 
#include <string.h>	// memset 
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include "sha512.h"
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
  sa_SIG.sa_handler = SIG_IGN;			// specify the signal handler function 
  sigemptyset(&sa_SIG.sa_mask);
  sa_SIG.sa_flags = 0;
  sigaction(signalNumber, &sa_SIG, NULL);	// register 
}

/**
 * Execute a piece of shell command without getting the result.
 *
 * @param shellCommand  The command to execute.
 */
void CUtil::executeShellCommand(std::string shellCommand) {
  FILE* p = popen(shellCommand.c_str(), "r");
  if (NULL != p) {
    fclose(p);
    p = NULL;
  }
  return;
}

/**
 * Get the path of the latest file under a specified dir.
 *
 * @param dir  The path of the directory.
 * @return The path of the latest file.
 */
string CUtil::getLatestFile(const string &dir) {
  int fileModTime = 0;
  string filePathWithLatestTS;  // newest file timestamp 

  struct dirent* ent = NULL;
  DIR *pDir = NULL;
  pDir = opendir(dir.c_str());
  while (NULL != (ent = readdir(pDir))) {
    if (ent->d_type==8) {
      string filePath = dir + "/" + ent->d_name;  // d_name is the file name, e.g. "1394271938.jpg" 
      time_t currentFileModTime = getFileModTime(filePath);
      if (currentFileModTime > fileModTime) {  // found a newer file 
        fileModTime = currentFileModTime;
        filePathWithLatestTS = filePath;
      }
    }
  }
  closedir(pDir);

  return filePathWithLatestTS;
}

/**
 * Get the last modification time of a file.
 *
 * @param filePath  The path of the file.
 * @return The timestamp of the file.
 */
time_t CUtil::getFileModTime(const std::string &filePath) {
  struct stat fileStat;
  memset(&fileStat, 0, sizeof(fileStat));

  if (0 == stat(filePath.c_str(), &fileStat)) {  // success 
    return fileStat.st_mtime;
  }
  return -1;
}

/**
 * Get the milliseconds elapsed since Jan. 1, 1970
 *
 * @return The timestamp value.
 */
long long CUtil::getMillisecond() {
  struct timeval tv;
  gettimeofday(&tv, NULL);

  return (long long) tv.tv_sec * 1000 + (long long) tv.tv_usec / 1000;
}

/**
 * Generate the validate string in the JSON-RPC request.
 *
 * @param hashWorkFactor  Defines how many times will be the result string hashed.
 * @param timestamp       The timestamp in the JSON-RPC request.
 * @param token           The token to encrypt the JSON-RPC request.
 * @return The validation string.
 */
string CUtil::generateValidateStr(unsigned int hashWorkFactor,
				  int timestamp,
				  string token) {
  stringstream ss;
  ss << timestamp << token;

  string result = sha512(ss.str());
  for (unsigned int i = 1; i < hashWorkFactor; i++) {
    result = sha512(result);
  }

  return result;
}
