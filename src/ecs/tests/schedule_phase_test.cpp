
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

TEST_F( SchedulePhaseTest, PhaseGraph )
{
    Registry registry;
    registry.ConfigurePhase(
        PhaseConfigureBuilder<UpdatePhase, PostUpdatePhase>().After<FirstPhase>().Before<LastPhase>().Chain().End() );
    registry.mScheduleManager->BuildGraph();
    auto graph = registry.mScheduleManager->GetScheduleGraph();
    auto topology = graph.GetTopology();
    EXPECT_EQ( topology.LayerNum(), 4 );
    EXPECT_EQ( topology.GetLayer( 0 ).size(), 1 );
    EXPECT_EQ( *topology.GetLayer( 0 ).begin(), PhaseIdRegistry::Get<FirstPhase>() );
    EXPECT_EQ( topology.GetLayer( 1 ).size(), 1 );
    EXPECT_EQ( *topology.GetLayer( 1 ).begin(), PhaseIdRegistry::Get<UpdatePhase>() );
    EXPECT_EQ( topology.GetLayer( 2 ).size(), 1 );
    EXPECT_EQ( *topology.GetLayer( 2 ).begin(), PhaseIdRegistry::Get<PostUpdatePhase>() );
    EXPECT_EQ( topology.GetLayer( 3 ).size(), 1 );
    EXPECT_EQ( *topology.GetLayer( 3 ).begin(), PhaseIdRegistry::Get<LastPhase>() );
}