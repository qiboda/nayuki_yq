
#include <gtest/gtest.h>

import ecs;
import ecs.default_phase;

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
    registry.ConfigurePhase( PhaseConfigure::Create<UpdatePhase, PostUpdatePhase>()
                                 .After<LastPhase>()
                                 .Before<FirstPhase>()
                                 .Chain()
                                 .Build() );
}

TEST_F( SchedulePhaseTest, PhaseGraph )
{
    Registry registry;
    registry.ConfigurePhase( PhaseConfigure::Create<UpdatePhase, PostUpdatePhase>()
                                 .After<FirstPhase>()
                                 .Before<LastPhase>()
                                 .Chain()
                                 .Build() );
    registry.GetScheduleManager()->ApplyPhaseConfigures();
    registry.GetScheduleManager()->BuildGraph();
    auto graph = registry.GetScheduleManager()->GetScheduleGraph();
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