#ifndef TST_REDANALYZERTESTCASE_H
#define TST_REDANALYZERTESTCASE_H


#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

TEST(RedAnalyzerTestSuiteName, RedAnalyzerTestCase)
{
    EXPECT_EQ(1, 1);
    ASSERT_THAT(0, Eq(0));
}

#endif // TST_REDANALYZERTESTCASE_H
