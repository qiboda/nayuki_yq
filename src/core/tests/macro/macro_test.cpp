#include <gtest/gtest.h>

#include <core/core.h>
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
    ASSERT_EXIT(
        NY_ASSERT_MSG( ( i == 4 ), "error: " ),
        []( i32 code ) -> bool
        {
            UNUSED_VAR( code );
            return true;
        },
        "" );
    ASSERT_DEATH( NY_ASSERT_MSG( ( i == 4 ), "error: " ), "" );
#endif
}