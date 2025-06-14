#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>

#include "ecs/schedule/config/node.h"
#include "ecs/schedule/config/system_set.h"
#include "ecs/schedule/graph/graph.h"
#include "ecs/systems/system_manager.h"

class ECS_API ScheduleGraph : public NonCopyable
{
  public:
    ScheduleGraph();

#pragma region Config
    /**
     * @brief
     * AddSystems(&SystemFunction)
     * @param config
     */
    void AddSystemNodeConfig( class ScheduleSystemNodeConfig&& config );

    void AddSystemSetNodeConfig( class ScheduleSystemSetNodeConfig&& config );

    void ApplyNodeConfigs();
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
#pragma endregion // InConfig

  void BuildCompositeGraph();
  void BuildDependencyGraph();

  protected:
    std::vector<ScheduleSystemNodeConfig> mSystemNodeConfigs;
    std::vector<ScheduleSystemSetNodeConfig> mSystemSetNodeConfigs;

  protected:
    std::unordered_map<SystemId, ScheduleNodeId> mAllSystemNodes;
    std::unordered_map<SystemSetId, ScheduleNodeId> mAllSystemSetNodes;

    std::unordered_map<ScheduleNodeId, ScheduleNodeVariant> mAllNodes;

    /// 用户设置的
    // 层级嵌套结构
    std::unordered_map<ScheduleNodeId, ScheduleNodeId> mCompositeEdges;
    // 依赖关系 水平结构
    std::unordered_map<ScheduleNodeId, ScheduleNodeId> mDependencyEdges;

  protected:
    /// 计算得出的
    // 此处的ScheduleNode只是一个占位符，因为Graph目前不需要Node
    Graph<ScheduleNodeId, ScheduleNode> mCompositeGraph;
    Graph<ScheduleNodeId, ScheduleNode> mDependencyGraph;

  protected:
    SystemManager* mSystemManager = nullptr;
    std::shared_ptr<class ScheduleBase> mSchedule = nullptr;
};

#include "ecs/schedule/config/system_node_config.h"
#include "ecs/schedule/config/system_set_node_config.h"

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
        return it.second;
    }

    auto scheduleNodeId = ScheduleNodeIdGenerator::Next();
    mAllSystemSetNodes.emplace( systemSetId, scheduleNodeId );
    mAllNodes.emplace( scheduleNodeId, ScheduleSystemSetNode( systemSetId ) );
    return scheduleNodeId;
}

template <IsSystemSetConcept T>
void ScheduleGraph::AfterSystemSetInConfig( ScheduleNodeId curNodeId )
{
    auto ScheduleNodeId = AddSystemSetInConfig<T>();

    mDependencyEdges.emplace( ScheduleNodeId, curNodeId );
}

template <IsSystemSetConcept T>
void ScheduleGraph::BeforeSystemSetInConfig( ScheduleNodeId curNodeId )
{
    auto ScheduleNodeId = AddSystemSetInConfig<T>();

    mDependencyEdges.emplace( curNodeId, ScheduleNodeId );
}

#pragma endregion // InConfig