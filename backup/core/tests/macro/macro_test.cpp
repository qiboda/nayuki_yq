#include <gtest/gtest.h>

import core;
#include <core/macro/macro.h>
#include <core/type_def.h>

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