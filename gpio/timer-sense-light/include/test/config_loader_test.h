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
    string timeRangeFile;
  
  protected:
    /**
     * Create a time range file for test.
     *
     * @param timeRangeFile The path of the file to create.
     */
    void createTimeRangeFile(const string &timeRangeFile) {
      vector<string> outputLines;
      outputLines.push_back("00:00\t00:35");
      outputLines.push_back("01:00\t06:00");
      ofstream ofs(timeRangeFile.c_str());
      ASSERT_TRUE(ofs.is_open()) << "Failed to open time range config file [" << timeRangeFile << "] to write";

      vector<string>::const_iterator it;
      for (it = outputLines.begin(); it != outputLines.end(); it++) {
	ofs << *it << endl;
      }
      ofs.close();
    }
    
    virtual void SetUp() {
      /* get current running program path */
      char path[PATH_MAX];
      memset(path, 0, sizeof(path));
      if (-1 != (int) CUtil::getExecutablePath(path, sizeof(path))) {
	currentAppPath = path;
      }

      // create time range config file
      timeRangeFile = currentAppPath + "/unit-test-time-range.txt";
      createTimeRangeFile(timeRangeFile);
    }

    virtual void TearDown() {
      remove(timeRangeFile.c_str());
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
    CConfigLoader loader;
    EXPECT_TRUE(loader.loadTimeRangeFromFile(timeRangeFile));
    
    for (int i = 0; i < ONE_DAY_MINUTES; i++) {
      if ((i >= 0 && i <= 35) || (i >= 60 && i <= 360)) {
	EXPECT_EQ(ENABLE_STATUS, loader.getTimePositionStatus(i)) << "Unexpected index [" << i << "] value";
      } else {
	EXPECT_EQ(DISABLE_STATUS, loader.getTimePositionStatus(i)) << "Unexpected index [" << i << "] value";
      }
    }
  }

  TEST_F(CConfigLoaderTest, givenEmptyTimeRangeStringLinesShouldReturnFalse) {
    CConfigLoader loader;
    EXPECT_TRUE(loader.loadTimeRangeFromFile(timeRangeFile));

    string timeRangeStrLines;
    EXPECT_FALSE(loader.updateTimeRange(timeRangeStrLines));
  }

  TEST_F(CConfigLoaderTest, givenValidTimeRangeStringLinesCouldUpdateToMemoryAndLocalFS) {
    CConfigLoader loader;
    EXPECT_TRUE(loader.loadTimeRangeFromFile(timeRangeFile));

    string timeRangeStrLines = "00:05\t00:20\n10:00\t12:00";
    EXPECT_TRUE(loader.updateTimeRange(timeRangeStrLines));

    for (int i = 0; i < ONE_DAY_MINUTES; i++) {
      if ((i >= 5 && i <= 20) || (i >= 600 && i <= 720)) {
  	EXPECT_EQ(ENABLE_STATUS, loader.getTimePositionStatus(i)) << "Unexpected index [" << i << "] value";
      } else {
  	EXPECT_EQ(DISABLE_STATUS, loader.getTimePositionStatus(i)) << "Unexpected index [" << i << "] value";
      }
    }
  }

  TEST_F(CConfigLoaderTest, givenInvalidTimePositionShouldReturnInvalidPositionFLag) {
    CConfigLoader loader;
    EXPECT_TRUE(loader.loadTimeRangeFromFile(timeRangeFile));

    EXPECT_EQ(INVALID_POSITION, loader.getTimePositionStatus(-1));
    EXPECT_EQ(INVALID_POSITION, loader.getTimePositionStatus(ONE_DAY_MINUTES + 1));
  }

  TEST_F(CConfigLoaderTest, givenLoadedTimeRangerDataShouldConvert2RightStrings) {
    CConfigLoader loader;
    EXPECT_TRUE(loader.loadTimeRangeFromFile(timeRangeFile));

    vector<string> output;
    string delimiter = " ";
    loader.translateTimeRange2String(output, delimiter);
    EXPECT_STREQ("00:00 00:35", output[0].c_str());
    EXPECT_STREQ("01:00 06:00", output[1].c_str());
  }
}

#endif
