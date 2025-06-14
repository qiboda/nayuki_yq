#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>

#include "ecs/schedule/config/nodes/node.h"
#include "ecs/schedule/config/system_set.h"
#include "ecs/systems/system_manager.h"

class ECS_API ScheduleGraph
{
  public:
    ScheduleGraph();

    /**
     * @brief
     * AddSystems(&SystemFunction)
     * @param config
     */
    void AddSystemNodeConfig( class ScheduleSystemNodeConfig&& config );

    void AddSystemSetNodeConfig( class ScheduleSystemSetNodeConfig&& config );

    void ApplyNodeConfigs();

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

  protected:
    std::vector<ScheduleSystemNodeConfig> mSystemNodeConfigs;
    std::vector<ScheduleSystemSetNodeConfig> mSystemSetNodeConfigs;

  protected:
    std::unordered_map<SystemId, ScheduleNodeId> AllSystemNodes;
    std::unordered_map<SystemSetId, ScheduleNodeId> AllSystemSetNodes;

    std::unordered_map<ScheduleNodeId, ScheduleNodeVariant> AllNodes;

    // 层级嵌套结构
    std::unordered_map<ScheduleNodeId, ScheduleNodeId> CompositeEdges;
    // 依赖关系 水平结构
    std::unordered_map<ScheduleNodeId, ScheduleNodeId> DependencyEdges;

  protected:
    SystemManager* mSystemManager = nullptr;
    std::shared_ptr<class ScheduleBase> mSchedule = nullptr;
};

#include "ecs/schedule/config/nodes/system_node.h"
#include "ecs/schedule/config/nodes/system_set_node.h"

template <IsSystemConcept Func>
ScheduleNodeId ScheduleGraph::AddSystemInConfig( Func func )
{
    SystemId systemId = mSystemManager->AddSystem( func );
    auto it = AllSystemNodes.find( systemId );
    if ( it != AllSystemNodes.end() )
    {
        // System already exists.
        return it->second;
    }

    auto scheduleNodeId = ScheduleNodeIdGenerator::Next();
    ScheduleSystemNode Node = ScheduleSystemNode( systemId );
    AllNodes.emplace( scheduleNodeId, std::move( Node ) );
    AllSystemNodes.emplace( systemId, scheduleNodeId );

    return scheduleNodeId;
}

// func -> curNodeId
template <IsSystemConcept Func>
void ScheduleGraph::AfterSystemInConfig( Func func, ScheduleNodeId curNodeId )
{
    ScheduleNodeId afterNodeId = AddSystemInConfig( func );
    DependencyEdges.emplace( afterNodeId, curNodeId );
}

template <IsSystemConcept Func>
void ScheduleGraph::BeforeSystemInConfig( ScheduleNodeId curNodeId, Func func )
{
    ScheduleNodeId beforeNodeId = AddSystemInConfig( func );
    DependencyEdges.emplace( curNodeId, beforeNodeId );
}

template <IsSystemSetConcept T>
void ScheduleGraph::InSetInConfig( ScheduleNodeId curNodeId )
{
    auto ScheduleNodeId = AddSystemSetInConfig<T>();

    CompositeEdges.emplace( ScheduleNodeId, curNodeId );
}

template <IsSystemSetConcept T>
ScheduleNodeId ScheduleGraph::AddSystemSetInConfig()
{
    auto systemSetId = SystemSetIdRegistry::Get<T>();
    auto it = AllSystemSetNodes.find( systemSetId );
    if ( it != AllSystemSetNodes.end() )
    {
        return it.second;
    }

    auto scheduleNodeId = ScheduleNodeIdGenerator::Next();
    AllSystemSetNodes.emplace( systemSetId, scheduleNodeId );
    AllNodes.emplace( scheduleNodeId, ScheduleSystemSetNode( systemSetId ) );
    return scheduleNodeId;
}

template <IsSystemSetConcept T>
void ScheduleGraph::AfterSystemSetInConfig( ScheduleNodeId curNodeId )
{
    auto ScheduleNodeId = AddSystemSetInConfig<T>();

    DependencyEdges.emplace( ScheduleNodeId, curNodeId );
}

template <IsSystemSetConcept T>
void ScheduleGraph::BeforeSystemSetInConfig( ScheduleNodeId curNodeId )
{
    auto ScheduleNodeId = AddSystemSetInConfig<T>();

    DependencyEdges.emplace( ScheduleNodeId, curNodeId );
}
