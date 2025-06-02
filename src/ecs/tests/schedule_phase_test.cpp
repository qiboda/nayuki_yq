
#include "ecs/registry.h"
#include "ecs/schedule/phase/default.h"
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
    auto value = std::format( "{}", PhaseBase{} );
    // std::cout << value << std::endl;

    Registry registry;
    registry.ConfigurePhase(
        PhaseConfigureBuilder<UpdatePhase, PostUpdatePhase>().After<LastPhase>().Before<FirstPhase>().Chain().End() );
}