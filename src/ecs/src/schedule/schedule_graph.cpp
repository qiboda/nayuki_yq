
#include "ecs/schedule/schedule_graph.h"
#include "ecs/schedule/graph/tarjan_graph.h"

ScheduleGraph::ScheduleGraph()
    : mSchedule( nullptr )
    , mSystemManager( nullptr )
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

bool ScheduleGraph::CheckCompositeGraphValid()
{
    TarjanGraph<ScheduleNodeId, ScheduleNode> tarjanGraph( mCompositeGraph );
    tarjanGraph.RunTarjan();
    const std::vector<std::vector<ScheduleNodeId>>& sccs = tarjanGraph.GetSCCs();
    for ( auto&& scc : sccs )
    {
        if ( scc.size() > 1 )
        {
            // 有环
            return false;
        }
    }
    return true;
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

void ScheduleGraph::SetScheduleBase( std::shared_ptr<class ScheduleBase> schedule )
{
    mSchedule = schedule;
}

void ScheduleGraph::Initialize()
{
    mSystemManager = std::make_shared<SystemManager>();
}

void ScheduleGraph::CleanUp()
{
}

void ScheduleGraph::ClearNodeConfigs()
{
    mSystemNodeConfigs.clear();
    mSystemSetNodeConfigs.clear();
}
