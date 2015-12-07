#ifndef	__CONFIG_LOADER_TEST_H
#define __CONFIG_LOADER_TEST_H

#include <gtest/gtest.h>
#include <linux/limits.h>  // PATH_MAX
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

  TEST_F(CConfigLoaderTest, givenExistingConfigFileShouldReturnTrue) {
    CConfigLoader loader;
    string existingConfigFile = currentAppPath + "/conf/main.conf";
    
    EXPECT_TRUE(loader.loadMainConfig(existingConfigFile));
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
}

#endif
