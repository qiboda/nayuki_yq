
#include "ecs/schedule/schedule_graph.h"

ScheduleGraph::ScheduleGraph()
{
}

void ScheduleGraph::AddSystemNodeConfig( ScheduleSystemNodeConfig&& config )
{
    mSystemNodeConfigs.push_back( std::move( config ) );
}

void ScheduleGraph::AddSystemSetNodeConfig( ScheduleSystemSetNodeConfig&& config )
{
    mSystemSetNodeConfigs.push_back( std::move( config ) );
}

void ScheduleGraph::ApplyNodeConfigs()
{
    for ( auto&& config : mSystemNodeConfigs )
    {
        config.Apply( mSchedule );
    }

    for ( auto&& config : mSystemSetNodeConfigs )
    {
        config.Apply( mSchedule );
    }
}

void ScheduleGraph::BuildCompositeGraph()
{
    for ( auto [outEdgeId, inEdgeId] : mCompositeEdges )
    {
        mCompositeGraph.AddNode( outEdgeId, {} );
        mCompositeGraph.AddNode( inEdgeId, {} );
        mCompositeGraph.AddEdge( outEdgeId, inEdgeId );
    }
}

void ScheduleGraph::BuildDependencyGraph()
{
    // for ( auto [outEdgeId, inEdgeId] : mCompositeEdges )
    // {
    //     mCompositeGraph.AddNode( outEdgeId, {} );
    //     mCompositeGraph.AddNode( inEdgeId, {} );
    //     mCompositeGraph.AddEdge( outEdgeId, inEdgeId );
    // }
}
