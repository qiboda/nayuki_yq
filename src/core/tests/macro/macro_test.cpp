#include <gtest/gtest.h>

#include <core/macro/macro.h>

import core;
import fmt;

class MacroTest : public ::testing::Test
{
  protected:
    MacroTest()
    {
    }

    ~MacroTest() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F( MacroTest, AssertPrint )
{
#ifndef NDEBUG
    auto i = 3;
    ASSERT_DEATH( NY_ASSERT_MSG( ( i == 4 ), "error: " ), "" );
#endif
}