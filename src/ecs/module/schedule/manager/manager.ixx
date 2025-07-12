module;

#include "module_export.h"
#include <fmt/format.h>

export module ecs.schedule.manager:manager;

import :phase;
// import :phase_config;
// import :phase_config_set;
// import :phase_default;

import ecs.schedule.graph;
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

    void ApplyPhaseConfigures();

    void BuildGraph()
    {
        mScheduleGraph.TopSort();
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
};
