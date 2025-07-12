#include <gtest/gtest.h>

import ecs.components.component;
import ecs.schedule.graph;
import ecs.schedule.tarjan_graph;
import ecs.schedule;
import ecs.schedule.manager;
import ecs.systems.params.query;

import core.misc.range;
import std;

class ScheduleGraphTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

  protected:
    struct Component1 : public Component
    {
    };

    static void System1( Query<Component1&>& )
    {
    }

    static void System2( Query<Component1&>& )
    {
    }

    static void System3( Query<Component1&>& )
    {
    }

    static void System4( Query<Component1&>& )
    {
    }

    struct SystemSet1 : public SystemSetBase
    {
    };

    struct SystemSet2 : public SystemSetBase
    {
    };

    struct SystemSet3 : public SystemSetBase
    {
    };
};

TEST_F( ScheduleGraphTest, TarjanTest )
{
    auto firstPhase = PhaseIdRegistry::Get<FirstPhase>();
    auto preUpdatePhase = PhaseIdRegistry::Get<PreUpdatePhase>();
    auto updatePhase = PhaseIdRegistry::Get<UpdatePhase>();
    auto postUpdatePhase = PhaseIdRegistry::Get<PostUpdatePhase>();
    auto lastPhase = PhaseIdRegistry::Get<LastPhase>();

    Graph<PhaseId, std::string> graph;
    graph.AddNode( firstPhase, std::string( "first phase" ) );
    graph.AddNode( preUpdatePhase, std::string( "pre update phase" ) );
    graph.AddNode( updatePhase, std::string( "update phase" ) );
    graph.AddNode( postUpdatePhase, std::string( "post update phase" ) );
    graph.AddNode( lastPhase, std::string( "last phase" ) );
    // 1 -> 2 -> 3 -> 4 -> 5
    // 5 -> 3
    // 2 -> 4
    graph.AddEdge( firstPhase, preUpdatePhase );
    graph.AddEdge( preUpdatePhase, updatePhase );
    graph.AddEdge( updatePhase, postUpdatePhase );
    graph.AddEdge( postUpdatePhase, lastPhase );
    graph.AddEdge( lastPhase, updatePhase );
    graph.AddEdge( preUpdatePhase, postUpdatePhase );

    TarjanGraph<PhaseId, std::string> tarjan( graph );
    tarjan.RunTarjan();
    auto SCCs = tarjan.GetSCCs();
    for ( auto vv : enumerate( SCCs ) )
    {
        for ( auto v : std::get<1>(vv) )
        {
            auto node = graph.GetNode( v );
            std::cout << std::get<0>(vv) << " " << node << std::endl;
        }
    }

    EXPECT_EQ( SCCs.size(), 3 );
    EXPECT_EQ( SCCs[0].size(), 3 );
    EXPECT_NE( std::find( SCCs[0].begin(), SCCs[0].end(), updatePhase ), SCCs[0].end() );
    EXPECT_NE( std::find( SCCs[0].begin(), SCCs[0].end(), lastPhase ), SCCs[0].end() );
    EXPECT_NE( std::find( SCCs[0].begin(), SCCs[0].end(), postUpdatePhase ), SCCs[0].end() );

    EXPECT_EQ( SCCs[1].size(), 1 );
    EXPECT_EQ( SCCs[1][0], preUpdatePhase );

    EXPECT_EQ( SCCs[2].size(), 1 );
    EXPECT_EQ( SCCs[2][0], firstPhase );
}

TEST_F( ScheduleGraphTest, SystemNodeConfig )
{
    std::shared_ptr<ScheduleBase> scheduleBase = std::make_shared<ScheduleBase>();
    scheduleBase->Initialize();
    auto scheduleGraph = scheduleBase->GetScheduleGraph();

    scheduleBase->AddSystemNodeConfig(
        ScheduleSystemNodeConfig::Create( &ScheduleGraphTest::System1 ).After<SystemSet1>().Build() );
    scheduleGraph->ApplyNodeConfigs();
    scheduleGraph->BuildCompositeGraph();
    scheduleGraph->BuildDependencyGraph();
    EXPECT_TRUE( scheduleGraph->CheckDependencyGraphValid() );

    scheduleGraph->ClearNodeConfigs();

    scheduleBase->AddSystemNodeConfig(
        ScheduleSystemNodeConfig::Create( &ScheduleGraphTest::System1 ).Before<SystemSet1>().Build() );
    scheduleGraph->ApplyNodeConfigs();
    scheduleGraph->BuildCompositeGraph();
    scheduleGraph->BuildDependencyGraph();
    // 节点之间没有依赖关系，所以是有效的。
    EXPECT_TRUE( scheduleGraph->CheckDependencyGraphValid() );

    scheduleGraph->ClearNodeConfigs();

    scheduleBase->AddSystemNodeConfig(
        ScheduleSystemNodeConfig::Create( &ScheduleGraphTest::System2 ).InSet<SystemSet1>().Build() );
    scheduleGraph->ApplyNodeConfigs();
    scheduleGraph->BuildCompositeGraph();
    scheduleGraph->BuildDependencyGraph();
    EXPECT_FALSE( scheduleGraph->CheckDependencyGraphValid() );
}

TEST_F( ScheduleGraphTest, SystemSetNodeConfig )
{
    std::shared_ptr<ScheduleBase> scheduleBase = std::make_shared<ScheduleBase>();
    scheduleBase->Initialize();
    auto scheduleGraph = scheduleBase->GetScheduleGraph();

    scheduleBase->AddSystemSetNodeConfig(
        ScheduleSystemSetNodeConfig::Create<SystemSet1>().InSet<SystemSet2>().Build() );
    scheduleGraph->ApplyNodeConfigs();
    scheduleGraph->BuildCompositeGraph();
    EXPECT_TRUE( scheduleGraph->CheckCompositeGraphValid() );

    scheduleGraph->ClearNodeConfigs();

    scheduleBase->AddSystemSetNodeConfig(
        ScheduleSystemSetNodeConfig::Create<SystemSet2>().InSet<SystemSet3>().Build() );
    scheduleGraph->ApplyNodeConfigs();
    scheduleGraph->BuildCompositeGraph();
    EXPECT_TRUE( scheduleGraph->CheckCompositeGraphValid() );

    scheduleGraph->ClearNodeConfigs();

    scheduleBase->AddSystemSetNodeConfig(
        ScheduleSystemSetNodeConfig::Create<SystemSet3>().InSet<SystemSet1>().Build() );
    scheduleGraph->ApplyNodeConfigs();
    scheduleGraph->BuildCompositeGraph();
    EXPECT_FALSE( scheduleGraph->CheckCompositeGraphValid() );
}