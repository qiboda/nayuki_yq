#pragma once

#include "ecs/schedule/graph/graph.h"
#include "ecs/schedule/phase/config.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

class ECS_API ScheduleManager
{
  public:
    ScheduleManager();

  public:
    // 期望有一个配置，可以转换为图的关系，设置到mScheduleGraph。
    void ConfigSchedulePhase( PhaseConfigure&& configure )
    {
        mPhaseConfigureSet.Configure( std::forward<PhaseConfigure>( configure ) );
    }

    const Graph<PhaseId, PhaseInfo>& GetScheduleGraph() const
    {
        return mScheduleGraph;
    }

    void BuildGraph()
    {
        mScheduleGraph = mPhaseConfigureSet.BuildGraph();
        mScheduleGraph.TopSort();
    }

  protected:
    Graph<PhaseId, PhaseInfo> mScheduleGraph;

    PhaseConfigureSet mPhaseConfigureSet;
};
