
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
    // 只是为了检测是否有环，不需要添加那些孤立的节点
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
    for ( auto [systemId, scheduleNodeId] : mAllSystemNodes )
    {
        mDependencyGraph.AddNode( scheduleNodeId, {} );
    }

    for ( auto [outEdgeId, inEdgeId] : mDependencyEdges )
    {
        auto outIt = mAllNodes.find( outEdgeId );
        NY_ASSERT( outIt != mAllNodes.end() );
        if ( outIt->second.Is<ScheduleSystemNode>() )
        {
            auto inIt = mAllNodes.find( inEdgeId );
            NY_ASSERT( inIt != mAllNodes.end() );
            if ( inIt->second.Is<ScheduleSystemNode>() )
            {
                mDependencyGraph.AddEdge( outEdgeId, inEdgeId );
            }
            else if ( inIt->second.Is<ScheduleSystemSetNode>() )
            {
                // 递归获得所有的SystemNode。
                std::vector<ScheduleNodeId> systemNodeIds = FindAllSystemNodesInSystemSet( inIt->first );
                for ( usize i = 0; i < systemNodeIds.size(); ++i )
                {
                    mDependencyGraph.AddEdge( outEdgeId, systemNodeIds[i] );
                }
            }
        }
        else if ( outIt->second.Is<ScheduleSystemSetNode>() )
        {
            auto inIt = mAllNodes.find( inEdgeId );
            NY_ASSERT( inIt != mAllNodes.end() );
            if ( inIt->second.Is<ScheduleSystemNode>() )
            {
                // 递归获得所有的SystemNode。
                std::vector<ScheduleNodeId> systemNodeIds = FindAllSystemNodesInSystemSet( outIt->first );
                for ( usize i = 0; i < systemNodeIds.size(); ++i )
                {
                    mDependencyGraph.AddEdge( systemNodeIds[i], inEdgeId );
                }
            }
            else
            {
                // 递归获得所有的SystemNode。
                std::vector<ScheduleNodeId> outSystemNodeIds = FindAllSystemNodesInSystemSet( outIt->first );
                std::vector<ScheduleNodeId> inSystemNodeIds = FindAllSystemNodesInSystemSet( inIt->first );
                for ( usize i = 0; i < outSystemNodeIds.size(); ++i )
                {
                    for ( usize j = 0; j < inSystemNodeIds.size(); ++j )
                    {
                        mDependencyGraph.AddEdge( outSystemNodeIds[i], inSystemNodeIds[j] );
                    }
                }
            }
        }
    }
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

std::vector<ScheduleNodeId> ScheduleGraph::FindAllSystemNodesInSystemSet( ScheduleNodeId systemSetNodeId )
{
    std::vector<ScheduleNodeId> systemScheduleNodeIds;

    auto it = mCompositeEdges.equal_range( systemSetNodeId );
    for ( auto iter = it.first; iter != it.second; ++iter )
    {
        auto scheduleNodeId = iter->second;
        auto nodeIt = mAllNodes.find( scheduleNodeId );
        NY_ASSERT_MSG( nodeIt != mAllNodes.end(), "SystemNodeId not found in all nodes" );
        if ( nodeIt->second.Is<ScheduleSystemNode>() )
        {
            systemScheduleNodeIds.push_back( scheduleNodeId );
        }
        else if ( nodeIt->second.Is<ScheduleSystemSetNode>() )
        {
            // 递归获得所有的SystemNode。
            auto subSystemNodeIds = FindAllSystemNodesInSystemSet( scheduleNodeId );
            systemScheduleNodeIds.insert( systemScheduleNodeIds.end(),
                                          subSystemNodeIds.begin(),
                                          subSystemNodeIds.end() );
        }
    }

    return systemScheduleNodeIds;
}

bool ScheduleGraph::CheckDependencyGraphValid()
{
    TarjanGraph<ScheduleNodeId, ScheduleNode> tarjanGraph( mDependencyGraph );
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

const Topology<ScheduleNodeId>& ScheduleGraph::GetTopology()
{
    mDependencyGraph.TopSort();
    return mDependencyGraph.GetTopology();
}
