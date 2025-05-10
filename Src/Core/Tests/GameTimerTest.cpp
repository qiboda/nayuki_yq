#include <gtest/gtest.h>

#include <Core/Core.h>

#include <Core/Timer/GameTimer.h>

#include <thread>

class GameTimerTest : public ::testing::Test
{
  protected:
    GameTimerTest() {}
    ~GameTimerTest() override {}

    void SetUp() override {}
    void TearDown() override {}

    GameTimer gameTimer;
};

TEST_F( GameTimerTest, GameTimerDeltaTime )
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for( 1s );
    gameTimer.Tick();
    ASSERT_GT( gameTimer.DeltaTime(), 1.0f );
    ASSERT_LT( gameTimer.DeltaTime(), 1.1f );
}

TEST_F( GameTimerTest, GameTimerTotalTime )
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for( 0.3s );
    gameTimer.Stop();
    gameTimer.Tick();
    std::this_thread::sleep_for( 0.4s );
    gameTimer.Start();
    gameTimer.Tick();
    std::this_thread::sleep_for( 0.7s );
    gameTimer.Tick();

    ASSERT_GT( gameTimer.RunningTotalTime(), 1.0f );
    ASSERT_LT( gameTimer.RunningTotalTime(), 1.1f );

    ASSERT_GT( gameTimer.PausedTotalTime(), 0.4f );
    ASSERT_LT( gameTimer.PausedTotalTime(), 0.42f );

    ASSERT_GT( gameTimer.TotalTime(), 1.4f );
    ASSERT_LT( gameTimer.TotalTime(), 1.5f );
}