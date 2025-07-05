#include <gtest/gtest.h>
#include "core/compiler/optimize.h"

import core;
import core.memory;

class MemoryTest : public ::testing::Test
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

TEST_F( MemoryTest, DefaultConstructor )
{
    Memory::Init();
    EXPECT_TRUE( Memory::IsInitted() );

    Memory::ThreadInit();
    EXPECT_TRUE( Memory::IsThreadInitted() );

    EXPECT_EQ( MemoryTracker::GetTotalAllocatedMemoryCount(), 0 );

    Memory::ThreadShutdown();
    EXPECT_FALSE( Memory::IsThreadInitted() );

    Memory::Shutdown();
    EXPECT_FALSE( Memory::IsInitted() );
}

NO_OPTIMIZE_BEGIN

TEST_F( MemoryTest, TestCount )
{
    Memory::Init();
    Memory::ThreadInit();

    EXPECT_EQ( MemoryTracker::GetTotalAllocatedMemoryCount(), 0 );
    EXPECT_EQ( MemoryTracker::GetCurrentAllocatedMemoryCount(), 0 );

    int* a = new int( 0 );

    EXPECT_EQ( MemoryTracker::GetTotalAllocatedMemoryCount(), 1 );
    EXPECT_EQ( MemoryTracker::GetCurrentAllocatedMemoryCount(), 1 );

    delete a;
    a = nullptr;

    EXPECT_EQ( MemoryTracker::GetTotalAllocatedMemoryCount(), 1 );
    EXPECT_EQ( MemoryTracker::GetCurrentAllocatedMemoryCount(), 0 );

    Memory::ThreadShutdown();
    Memory::Shutdown();
}

NO_OPTIMIZE_END