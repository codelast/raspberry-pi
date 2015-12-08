#ifndef	__CONFIG_LOADER_TEST_H
#define __CONFIG_LOADER_TEST_H

#include <gtest/gtest.h>
#include <linux/limits.h>  // PATH_MAX
#include <fstream>         // std::ofstream
#include <algorithm>       // std::fill_n()
#include <stdio.h>         // remove()
#include "config_loader.h"

/**
 * class CConfigLoader tester.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

namespace {
  class CConfigLoaderTest : public testing::Test
  {
  protected:
    string currentAppPath;
  
  protected:
    virtual void SetUp() {
      /* get current running program path */
      char path[PATH_MAX];
      memset(path, 0, sizeof(path));
      if (-1 != (int) CUtil::getExecutablePath(path, sizeof(path))) {
	currentAppPath = path;
      }
    }
  
    virtual void TearDown() {
    }
  };

  TEST_F(CConfigLoaderTest, givenNonExistConfigFileShouldReturnFalse) {
    CConfigLoader loader;
    string nonExistConfigFile = currentAppPath + "/non-exist.conf";

    EXPECT_FALSE(loader.loadMainConfig(nonExistConfigFile));
  }

  TEST_F(CConfigLoaderTest, givenValidConfigFileShouldLoadRightData) {
    CConfigLoader loader;
    string validConfigFile = currentAppPath + "/conf/main.conf";
    
    EXPECT_TRUE(loader.loadMainConfig(validConfigFile));
    EXPECT_EQ(0, loader.getPyroelectricGpioPort());
    EXPECT_EQ(1, loader.getLedGpioPortStart());
    EXPECT_EQ(4, loader.getLedNumber());
    EXPECT_EQ(80, loader.getListenPort());
    string expectedWebRootPath = currentAppPath + "/web-root";
    EXPECT_STREQ(expectedWebRootPath.c_str(), loader.getWebRootPath().c_str());
  }

  TEST_F(CConfigLoaderTest, givenNonExistTimeRangeFileShouldReturnFalse) {
    CConfigLoader loader;
    string nonExistTimeRangeFile = currentAppPath + "/non-exist-time-range-file.txt";

    EXPECT_FALSE(loader.loadTimeRangeFromFile(nonExistTimeRangeFile));
  }

  TEST_F(CConfigLoaderTest, givenValidTimeRangeFileShouldLoadRightData) {
    /* create a temp time range file for test */
    string validTimeRangeFile = currentAppPath + "/unit-test-time-range.txt";
    vector<string> outputLines;
    outputLines.push_back("00:00\t00:35");
    outputLines.push_back("01:00\t06:00");
    ofstream ofs(validTimeRangeFile.c_str());
    ASSERT_TRUE(ofs.is_open()) << "Failed to open time range config file [" << validTimeRangeFile << "] to write";

    vector<string>::const_iterator it;
    for (it = outputLines.begin(); it != outputLines.end(); it++) {
      ofs << *it << endl;
    }
    ofs.close();

    CConfigLoader loader;

    EXPECT_TRUE(loader.loadTimeRangeFromFile(validTimeRangeFile));
    
    int* actualTimeRangeArray = loader.getTimeRangeArray();
    for (int i = 0; i < ONE_DAY_MINUTES; i++) {
      if ((i >= 0 && i <= 35) || (i >= 60 && i <= 360)) {
	EXPECT_EQ(ENABLE_STATUS, actualTimeRangeArray[i]) << "Unexpected index [" << i << "] value";
      } else {
	EXPECT_EQ(DISABLE_STATUS, actualTimeRangeArray[i]) << "Unexpected index [" << i << "] value";
      }
    }

    // cleanup
    remove(validTimeRangeFile.c_str());
  }
}

#endif
