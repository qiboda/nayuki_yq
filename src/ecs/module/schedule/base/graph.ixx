module;

#include "module_export.h"

export module ecs.schedule:schedule_graph;

import :config_node;
import :system_set;
// import :forward;
// import :node_config_cache;
// import :system_node_config;
// import :system_set_node_config;

import ecs.schedule.graph;
import ecs.systems.manager;
import ecs.systems.concepts;
import ecs.systems.id;

import std;
import core.type;
import core.misc.iraii;

export class ECS_API ScheduleGraph : public IRAII
{
  public:
    ScheduleGraph();

    virtual ~ScheduleGraph() override
    {
    }

    void SetScheduleBase( std::shared_ptr<class ScheduleBase> schedule );

  public:
    virtual void Initialize() override;
    virtual void CleanUp() override;

  public:
#pragma region Config
    /**
     * @brief
     * AddSystems(&SystemFunction)
     * @param config
     */
    void AddSystemNodeConfig( class ScheduleSystemNodeConfig&& config );

    void AddSystemSetNodeConfig( class ScheduleSystemSetNodeConfig&& config );

    void ApplyNodeConfigs();
    void ClearNodeConfigs();
#pragma endregion Config

#pragma region InConfig
    template <IsSystemConcept Func>
    ScheduleNodeId AddSystemInConfig( Func func );

    template <IsSystemConcept Func>
    void AfterSystemInConfig( Func func, ScheduleNodeId curNodeId );

    template <IsSystemConcept Func>
    void BeforeSystemInConfig( ScheduleNodeId curNodeId, Func func );

    template <IsSystemSetConcept T>
    void InSetInConfig( ScheduleNodeId curNodeId );

    template <IsSystemSetConcept T>
    ScheduleNodeId AddSystemSetInConfig();

    template <IsSystemSetConcept T>
    void AfterSystemSetInConfig( ScheduleNodeId curNodeId );

    template <IsSystemSetConcept T>
    void BeforeSystemSetInConfig( ScheduleNodeId curNodeId );

    void ChainInConfig( ScheduleNodeIdChainType&& Chain )
    {
        for ( usize i = 0; i < Chain.size() - 1; ++i )
        {
            auto curNodeId = Chain[i];
            auto nextNodeId = Chain[i + 1];
            mDependencyEdges.emplace( curNodeId, nextNodeId );
        }
    }

#pragma endregion // InConfig

    void BuildGraph();

    void BuildCompositeGraph();
    bool CheckCompositeGraphValid();

    void BuildDependencyGraph();
    bool CheckDependencyGraphValid();

    const Topology<ScheduleNodeId>& GetTopology();

  protected:
    std::vector<ScheduleNodeId> FindAllSystemNodesInSystemSet( ScheduleNodeId systemSetNodeId );

  protected:
    std::shared_ptr<struct NodeConfigCache> mNodeConfigCache;

  protected:
    std::unordered_map<SystemId, ScheduleNodeId> mAllSystemNodes;
    std::unordered_map<SystemSetId, ScheduleNodeId> mAllSystemSetNodes;

    std::unordered_map<ScheduleNodeId, ScheduleNodeVariant> mAllNodes;

    /// 用户设置的
    // 层级嵌套结构，因为一个父节点可以有多个子节点，所以是multimap
    std::unordered_multimap<ScheduleNodeId, ScheduleNodeId> mCompositeEdges;
    // 依赖关系 水平结构，一个节点可以有多个相连节点，所以是multimap
    std::unordered_multimap<ScheduleNodeId, ScheduleNodeId> mDependencyEdges;

  protected:
    /// 计算得出的
    // 此处的ScheduleNode只是一个占位符，因为Graph目前不需要Node
    Graph<ScheduleNodeId, ScheduleNode> mCompositeGraph;
    Graph<ScheduleNodeId, ScheduleNode> mDependencyGraph;

  protected:
    std::shared_ptr<ScheduleBase> mSchedule = nullptr;
    std::shared_ptr<SystemManager> mSystemManager = nullptr;
};

#pragma region InConfig

template <IsSystemConcept Func>
ScheduleNodeId ScheduleGraph::AddSystemInConfig( Func func )
{
    SystemId systemId = mSystemManager->AddSystem( func );
    auto it = mAllSystemNodes.find( systemId );
    if ( it != mAllSystemNodes.end() )
    {
        // System already exists.
        return it->second;
    }

    auto scheduleNodeId = ScheduleNodeIdGenerator::Next();
    ScheduleSystemNode Node = ScheduleSystemNode( systemId );
    mAllNodes.emplace( scheduleNodeId, std::move( Node ) );
    mAllSystemNodes.emplace( systemId, scheduleNodeId );

    return scheduleNodeId;
}

// func -> curNodeId
template <IsSystemConcept Func>
void ScheduleGraph::AfterSystemInConfig( Func func, ScheduleNodeId curNodeId )
{
    ScheduleNodeId afterNodeId = AddSystemInConfig( func );
    mDependencyEdges.emplace( afterNodeId, curNodeId );
}

template <IsSystemConcept Func>
void ScheduleGraph::BeforeSystemInConfig( ScheduleNodeId curNodeId, Func func )
{
    ScheduleNodeId beforeNodeId = AddSystemInConfig( func );
    mDependencyEdges.emplace( curNodeId, beforeNodeId );
}

template <IsSystemSetConcept T>
void ScheduleGraph::InSetInConfig( ScheduleNodeId curNodeId )
{
    auto ScheduleNodeId = AddSystemSetInConfig<T>();

    mCompositeEdges.emplace( ScheduleNodeId, curNodeId );
}

template <IsSystemSetConcept T>
ScheduleNodeId ScheduleGraph::AddSystemSetInConfig()
{
    auto systemSetId = SystemSetIdRegistry::Get<T>();
    auto it = mAllSystemSetNodes.find( systemSetId );
    if ( it != mAllSystemSetNodes.end() )
    {
        return it->second;
    }

    auto scheduleNodeId = ScheduleNodeIdGenerator::Next();
    mAllSystemSetNodes.emplace( systemSetId, scheduleNodeId );
    mAllNodes.emplace( scheduleNodeId, ScheduleSystemSetNode( systemSetId ) );
    return scheduleNodeId;
}

template <IsSystemSetConcept T>
void ScheduleGraph::AfterSystemSetInConfig( ScheduleNodeId curNodeId )
{
    auto scheduleNodeId = AddSystemSetInConfig<T>();

    mDependencyEdges.emplace( scheduleNodeId, curNodeId );
}

template <IsSystemSetConcept T>
void ScheduleGraph::BeforeSystemSetInConfig( ScheduleNodeId curNodeId )
{
    auto scheduleNodeId = AddSystemSetInConfig<T>();

    mDependencyEdges.emplace( curNodeId, scheduleNodeId );
}

#pragma endregion // InConfig