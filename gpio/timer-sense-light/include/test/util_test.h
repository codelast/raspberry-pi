#ifndef	__UTIL_TEST_H
#define __UTIL_TEST_H

#include <gtest/gtest.h>
#include <linux/limits.h>  // PATH_MAX
#include <sys/stat.h>      // mkdir()
#include <sys/types.h>     // mkdir()
#include <stdlib.h>        // system()
#include <stdio.h>         // remove()
#include "constants.h"
#include "util.h"

/**
 * class CUtil tester.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

namespace {
  class CUtilTest : public testing::Test
  {
  protected:
    string currentAppPath;
    string toCreateDir;
  
  protected:
    virtual void SetUp() {
      /* get current running program path */
      char path[PATH_MAX];
      memset(path, 0, sizeof(path));
      if (-1 != (int) CUtil::getExecutablePath(path, sizeof(path))) {
	currentAppPath = path;
      }
      toCreateDir = currentAppPath + "to-create-dir-for-ut";

      /* remove the dir before creating it(if it exists) */
      string shellCommand = "rm -rf " + toCreateDir;
      system(shellCommand.c_str());
    }
  
    virtual void TearDown() {
      remove(toCreateDir.c_str());
    }

    /**
     * Get the output of a piece of shell command.
     *
     * @param shellCommand The shell command to execute, e.g. "ls"
     * @return The output of the command.
     */
    string getShellCommandOutput(const std::string shellCommand) {
      FILE* crs = popen(shellCommand.c_str(), "r"); // execute the shell command
      char result[1024] = "0";
      fread(result, sizeof(char), sizeof(result), crs);
      if (NULL != crs) {
	fclose(crs);
	crs = NULL;
      }
      std::string res = result;
      return res;
    }
  };

  TEST_F(CUtilTest, twoImplementationOfCurrentAppPathShouldEquals) {
    // shell command "pwd" returns current path, but ends with a "\n"
    string currentPathByShellCmd = getShellCommandOutput("pwd");
    EXPECT_STREQ(currentPathByShellCmd.substr(0, currentPathByShellCmd.length() - 1).c_str(),
		 currentAppPath.c_str());
  }

  TEST(isDirExistTest, givenNonExistDirShouldReturnFalse) {
    EXPECT_FALSE(CUtil::isDirExist("a-non-exist-dir"));
  }

  TEST_F(CUtilTest, givenExistingDirShouldReturnTrue) {
    /* create a temp directory for unit test */
    int ret = mkdir(toCreateDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    ASSERT_EQ(0, ret) << "Failed to create dir [" << toCreateDir << "], return value: " << ret;
  
    EXPECT_TRUE(CUtil::isDirExist(toCreateDir));
  }

  TEST(stringSplitTest, givenEmptyInputStringShouldGetEmptyOutput) {
    string input = "";
    vector<string> output;
    CUtil::stringSplit(input, '\t', output);

    EXPECT_EQ(0, output.size());
  }

  TEST(stringSplitTest, givenNonEmptyInputStringShouldSplitToSubStrings) {
    string input = "a\tb\tc";
    vector<string> output;
    CUtil::stringSplit(input, '\t', output);

    EXPECT_EQ(3, output.size());
    EXPECT_STREQ("a", output[0].c_str());
    EXPECT_STREQ("b", output[1].c_str());
    EXPECT_STREQ("c", output[2].c_str());
  }

  TEST(getPositionInTimeRangeTest, givenValidInputStringShouldReturnInvalidFlag) {
    string hourAndMinute("123");
    EXPECT_EQ(INVALID_POSITION, CUtil::getPositionInTimeRange(hourAndMinute));
  }

  TEST(getPositionInTimeRangeTest, givenInvalidInputStringShouldReturnRightPosition) {
    string hourAndMinute("00:05");
    EXPECT_EQ(5, CUtil::getPositionInTimeRange(hourAndMinute));

    hourAndMinute = "10:20";
    EXPECT_EQ(620, CUtil::getPositionInTimeRange(hourAndMinute));
  }
}

#endif
