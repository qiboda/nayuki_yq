#include <gtest/gtest.h>

#include <NayukiYq/NayukiYq.h>

class MacroTest
    : public ::testing::Test
{
protected:
    MacroTest() {}
    ~MacroTest() override {}

    void SetUp() override {}
    void TearDown() override {}
};

TEST_F( MacroTest, AssertPrint )
{
    ASSERT_EXIT(
        NY_ASSERT_MSG( ( 3 == 4 ), L"error: %d", 10 ),
        []( i32 code ) -> bool
        {
            UNUSED_VAR(code);
            return true;
        },
        ""
    );
    ASSERT_DEATH( NY_ASSERT_MSG( ( 3 == 4 ), L"error: %d", 10 ), "" );
}