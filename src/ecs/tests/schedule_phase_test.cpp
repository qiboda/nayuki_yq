
#include "ecs/registry.h"
#include "ecs/schedule/phase/default.h"
#include <gtest/gtest.h>

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

TEST_F( SchedulePhaseTest, PhaseConfigure )
{
    Registry registry;
    registry.ConfigurePhase(
        PhaseConfigureBuilder<UpdatePhase, PostUpdatePhase>().After<LastPhase>().Before<FirstPhase>().Chain().End() );
}