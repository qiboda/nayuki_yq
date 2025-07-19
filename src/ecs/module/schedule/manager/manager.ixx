module;

#include "module_export.h"
#include <core/macro/macro.h>
#include <fmt/format.h>

export module ecs:schedule_manager;

import :schedule_phase;
import :schedule_base;

import ecs.graph;
import std;

export class ECS_API ScheduleManager : public std::enable_shared_from_this<ScheduleManager>
{
  public:
    ScheduleManager();

    ~ScheduleManager();

  public:
    // 期望有一个配置，可以转换为图的关系，设置到mScheduleGraph。
    void ConfigSchedulePhase( struct PhaseConfigure&& configure );

    const Graph<PhaseId, PhaseInfo>& GetScheduleGraph() const
    {
        return mScheduleGraph;
    }

    /// 添加一个系统节点配置到调度图中。
    template <IsSchedulePhase T>
    void AddSystemNodeConfig( class ScheduleSystemNodeConfig&& config )
    {
        if ( mPhaseScheduleMap.find( PhaseIdRegistry::Get<T>() ) == mPhaseScheduleMap.end() )
        {
            auto schedule = std::make_shared<ScheduleBase>();
            mPhaseScheduleMap[PhaseIdRegistry::Get<T>()] = schedule;
        }
        mPhaseScheduleMap[PhaseIdRegistry::Get<T>()]->AddSystemNodeConfig( std::move( config ) );
    }

    /// 添加一个系统集节点配置到调度图中。
    template <IsSchedulePhase T>
    void AddSystemSetNodeConfig( class ScheduleSystemSetNodeConfig&& config )
    {
        if ( mPhaseScheduleMap.find( PhaseIdRegistry::Get<T>() ) == mPhaseScheduleMap.end() )
        {
            auto schedule = std::make_shared<ScheduleBase>();
            mPhaseScheduleMap[PhaseIdRegistry::Get<T>()] = schedule;
        }
        mPhaseScheduleMap[PhaseIdRegistry::Get<T>()]->AddSystemSetNodeConfig( std::move( config ) );
    }

    void ApplyPhaseConfigures();

    void BuildGraph()
    {
        mScheduleGraph.TopSort();
    }

    void BeforeRun( Registry* registry )
    {
        ApplyPhaseConfigures();
        BuildGraph();
        for ( auto&& [phaseId, schedule] : mPhaseScheduleMap )
        {
            UNUSED_VARS( phaseId );
            if ( schedule )
            {
                schedule->Init( registry );
                schedule->BeforeRun( registry );
            }
        }
    }

  public:
    void Run( Registry* registry )
    {
        BeforeRun( registry );
        while ( true )
        {
            auto topology = mScheduleGraph.GetTopology();
            for ( usize i = 0; i < topology.LayerNum(); ++i )
            {
                auto& layer = topology.GetLayer( i );
                for ( auto&& phaseId : layer )
                {
                    auto phaseScheduleIt = mPhaseScheduleMap.find( phaseId );
                    if ( phaseScheduleIt != mPhaseScheduleMap.end() )
                    {
                        auto schedule = phaseScheduleIt->second;
                        if ( schedule )
                        {
                            schedule->Run( registry );
                        }
                    }
                }
            }
        }
    }

    template <IsSchedulePhase T>
    PhaseId AddPhaseInConfig()
    {
        PhaseId id = PhaseIdRegistry::Get<T>();
        mScheduleGraph.AddNode( id, PhaseInfo{ .mId = id, .mName = fmt::format( "{}", T{} ) } );
        return id;
    }

    template <IsSchedulePhase T>
    void AfterPhaseInConfig( PhaseId phaseId )
    {
        PhaseId afterId = AddPhaseInConfig<T>();
        mScheduleGraph.AddEdge( afterId, phaseId );
    }

    template <IsSchedulePhase T>
    void BeforePhaseInConfig( PhaseId phaseId )
    {
        PhaseId beforeId = AddPhaseInConfig<T>();
        mScheduleGraph.AddEdge( phaseId, beforeId );
    }

    void ChainInConfig( PhaseIdChainType&& chain );

  protected:
    std::shared_ptr<class PhaseConfigSet> mPhaseConfigSet;

    Graph<PhaseId, PhaseInfo> mScheduleGraph;
    std::map<PhaseId, std::shared_ptr<ScheduleBase>> mPhaseScheduleMap;
};
