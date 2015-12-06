#ifndef	__CONFIG_LOADER_TEST_H
#define __CONFIG_LOADER_TEST_H

#include <gtest/gtest.h>
#include <linux/limits.h>  // PATH_MAX
#include <stdlib.h>        // system()
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
    string testDataDir;
  
  protected:
    virtual void SetUp() {
      /* get current running program path */
      string currentAppPath;
      char path[PATH_MAX];
      memset(path, 0, sizeof(path));
      if (-1 != (int) CUtil::getExecutablePath(path, sizeof(path))) {
	currentAppPath = path;
      }
      testDataDir = currentAppPath + "test";

      /* create the test data dir */
      string shellCommand = "mkdir -p " + testDataDir;
      system(shellCommand.c_str());
    }
  
    virtual void TearDown() {
      remove(testDataDir.c_str());
    }
  };

  TEST_F(CConfigLoaderTest, givenNonExistConfigFileShouldReturnFalse) {
    CConfigLoader loader;
    string nonExistConfigFile = testDataDir + "/main.conf";
    EXPECT_FALSE(loader.loadMainConfig(nonExistConfigFile));
  }
}

#endif
