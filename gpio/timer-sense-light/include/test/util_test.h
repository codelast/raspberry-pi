#ifndef	__UTIL_TEST_H
#define __UTIL_TEST_H

#include <gtest/gtest.h>
#include <linux/limits.h>  // PATH_MAX
#include <sys/stat.h>      // mkdir()
#include <sys/types.h>     // mkdir()
#include <stdlib.h>        // system()
#include <stdio.h>         // remove()
#include "util.h"

/**
 * class CUtil tester.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

class CUtilTest : public testing::Test
{
 protected:
  string toCreateDir;
  
 protected:
  virtual void SetUp() {
    /* get current running program path */
    string currentAppPath;
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
};

TEST_F(CUtilTest, isDirExistTestGivenExistingDirShouldReturnTrue) {
  /* create a temp directory for unit test */
  int ret = mkdir(toCreateDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  ASSERT_EQ(0, ret) << "Failed to create dir [" << toCreateDir << "], return value: " << ret;
  
  EXPECT_TRUE(CUtil::isDirExist(toCreateDir));
}

TEST(isDirExistTest, isDirExistTestGivenNonExistDirShouldReturnFalse) {
  EXPECT_FALSE(CUtil::isDirExist("a-non-exist-dir"));
}

TEST(stringSplitTest, givenValidStringShouldSplitToSubStrings) {
  string input = "a\tb\tc";
  vector<string> output;
  CUtil::stringSplit(input, '\t', output);

  EXPECT_EQ(3, output.size());
  EXPECT_STREQ("a", output[0].c_str());
  EXPECT_STREQ("b", output[1].c_str());
  EXPECT_STREQ("c", output[2].c_str());
}

#endif
