#include <unistd.h>		// readlink() 
#include <linux/limits.h>	// PATH_MAX 
#include <string.h>		// memset 
#include <iostream>
#include <libconfig.h++>
#include "config.h"
#include "constants.h"
#include "util.h"

/**
 * This class stores the configuration of this program.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;
using namespace libconfig;

CConfig::CConfig()
{
  threadRunning = true;
  pinA = 0;
  pinB = 1;
  pinC = 2;
  pinD = 3;

  // get current running program path
  char path[PATH_MAX];
  memset(path, 0, sizeof(path));
  if (-1 != (int) getExecutablePath(path, sizeof(path))) {
    currentAppPath = path;
  }
}

CConfig::~CConfig() {
}

/**
 * Get the path of current program.
 *
 * @param buffer	Returned path be placed in.
 * @param len		Returned path length.
 * @return The number of characters in the path, -1 on error.
 */
size_t CConfig::getExecutablePath(char* buffer, size_t len) {
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
 * Load configuration from a file.
 *
 * @return true for successfully loaded the config file, false otherwise.
 */
bool CConfig::loadConfig(const string configFile) {
  Config cfg;

  try {
    cfg.readFile(configFile.c_str());
  } catch(const FileIOException &fioex) {    
    cout << "I/O exception when loading config file." << endl;
    return false;
  } catch(const ParseException &pex) {
    cout << "Parse config file error at " << pex.getFile() << " : " << pex.getLine() << " - " << pex.getError() << endl;
    return false;
  }

  // get the root object of the config object 
  const Setting& root = cfg.getRoot();

  try {
    const Setting &server = root[CFG_FILE_SECTION_SERVER.c_str()];

    if (!(server.lookupValue("ListenPort", listenPort) && server.lookupValue("Token", token)
	  && server.lookupValue("HashWorkFactor", hashWorkFactor))) {
      cout << "Failed to read config file section " << CFG_FILE_SECTION_SERVER << endl;
    }
  } catch (...) {
    cout << "Exception caught when reading config file section " << CFG_FILE_SECTION_SERVER << endl;
    return false;
  }

  try {
    const Setting &local = root[CFG_FILE_SECTION_LOCAL.c_str()];

    if (!(local.lookupValue("ImageDir", imageDir) && local.lookupValue("WebcamIndex", webcamIndex)
	  && local.lookupValue("PinA", pinA) && local.lookupValue("PinB", pinB)
	  && local.lookupValue("PinC", pinC) && local.lookupValue("PinD", pinD))) {
      cout << "Failed to read config file section " << CFG_FILE_SECTION_LOCAL << endl;
    }
  } catch (...) {
    cout << "Exception caught when reading config file section " << CFG_FILE_SECTION_LOCAL << endl;
    return false;
  }

  fullImageDir = currentAppPath + "/" + imageDir;
  // create the image dir by executing the mkdir shell command
  CUtil::executeShellCommand("mkdir -p " + fullImageDir);
  return true;
}
