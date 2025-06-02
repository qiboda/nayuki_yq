
#include "ecs/schedule/phase/phase.h"
#include <gtest/gtest.h>

#include <format>

class SchedulePhaseTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F( SchedulePhaseTest, Format )
{
    // auto value = std::format( "{}", SchedulePhaseBase{} );
    // std::cout << value << std::endl;
}