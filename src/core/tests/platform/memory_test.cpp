#include <gtest/gtest.h>

// #include <core/platform/memory.h>
import core.platform.memory;

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
    PlatformMemory Memory;
    EXPECT_EQ( Memory.GetCacheLineSize(), 64 );
}