#include "ecs/schedule/graph/graph.h"
#include "ecs/schedule/graph/tarjan_graph.h"
#include "ecs/schedule/phase/default.h"
#include "ecs/schedule/phase/phase.h"
#include <gtest/gtest.h>
#include <range/v3/view/enumerate.hpp>

class ScheduleGraphTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
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
    for ( auto vv : SCCs | ranges::views::enumerate )
    {
        for ( auto v : vv.second )
        {
            auto node = graph.GetNode( v );
            std::cout << vv.first << " " << node << std::endl;
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