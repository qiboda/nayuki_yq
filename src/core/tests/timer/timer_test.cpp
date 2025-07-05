#include <gtest/gtest.h>

import core.timer.manager;
import core.type;

class TypeIdTest : public ::testing::Test
{
  protected:
    TypeIdTest()
    {
    }

    ~TypeIdTest() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F( TypeIdTest, TimerMultipleExecute )
{
    i32 i = 0;
    TimerDelegate AddOne;
    AddOne.Bind(
        [&i]()
        {
            i++;
        } );
    TimerManager::GetInstance().SetTimer( 1, std::move( AddOne ), true );

    TimerManager::GetInstance().Tick( 79.7f );
    ASSERT_EQ( i, 79 );

    TimerManager::GetInstance().Tick( 20.3f );
    ASSERT_EQ( i, 100 );
}