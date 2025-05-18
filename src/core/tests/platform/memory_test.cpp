#include <gtest/gtest.h>

#include <core/platform/memory.h>

class PlatformMemoryTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // This function will be called before each test.
    }

    void TearDown() override
    {
        // This function will be called after each test.
    }
};

TEST_F( PlatformMemoryTest, DefaultConstructor )
{
    Memory Memory;
    EXPECT_EQ( Memory.GetCacheLineSize(), 64 );
}