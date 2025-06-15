
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

/**
 * @brief
 * 没有同一个元素，且无序的两两元素遍历。
 * 即没有 (a, a) 以及 (a, b) 和 (b, a) 都会遍历 的情况。
 */
template <typename T, typename Func>
void ForEachPair( const std::set<T>& s, Func&& f )
{
    for ( auto it1 = s.begin(); it1 != s.end(); ++it1 )
    {
        auto it2 = it1;
        ++it2;
        for ( ; it2 != s.end(); ++it2 )
        {
            f( *it1, *it2 );
        }
    }
}

void ScheduleGraph::BuildGraph()
{
    // 用户定义的嵌套结构循环检测
    BuildCompositeGraph();
    if ( CheckCompositeGraphValid() == false )
    {
        return;
    }

    // TODO: 应该增加一个展开前的检测。
    // 比如，设置了system set，但是system set内部没有节点，展开后的循环检测检测不到。
    // 但是外部插件可能会在内部设置节点，就总是会出错了。应该提前检测，避免外部拓展出问题。

    // 用户定义的展开后的 node 循环检测
    BuildDependencyGraph();
    if ( CheckDependencyGraphValid() == false )
    {
        return;
    }

    const Topology<ScheduleNodeId>& topology = GetTopology();
    for ( usize i = 0; i < topology.LayerNum(); ++i )
    {
        const std::set<ScheduleNodeId>& layer = topology.GetLayer( i );

        ForEachPair( layer,
                     [this]( const ScheduleNodeId& nodeId1, const ScheduleNodeId& nodeId2 )
                     {
                         auto it1 = mAllNodes.find( nodeId1 );
                         auto it2 = mAllNodes.find( nodeId2 );

                         auto& node1 = it1->second.Get<ScheduleSystemNode>();
                         auto& node2 = it2->second.Get<ScheduleSystemNode>();

                         auto& system1 = mSystemManager->GetSystem( node1.GetSystemId() );
                         auto& system2 = mSystemManager->GetSystem( node2.GetSystemId() );

                         if ( system1->write_conflict( system2 ) )
                         {
                             mDependencyGraph.AddEdge( nodeId1, nodeId2 );
                         }

                         if ( system2->write_conflict( system1 ) )
                         {
                             mDependencyGraph.AddEdge( nodeId2, nodeId1 );
                         }
                     } );
    }
}
