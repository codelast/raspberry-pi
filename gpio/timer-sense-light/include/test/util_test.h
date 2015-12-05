#include <gtest/gtest.h>
#include "util.h"

/**
 * util.cpp tester.
 *
 * @author Darran Zhang @ codelast.com
 */

using namespace std;

TEST(stringSplitTest, givenValidStringShouldSplitToSubStrings) {
  string input = "a\tb\tc";
  vector<string> output;
  CUtil::stringSplit(input, '\t', output);
  EXPECT_EQ(3, output.size());
}
