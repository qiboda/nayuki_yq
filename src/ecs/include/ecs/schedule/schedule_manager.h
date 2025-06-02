#pragma once

#include "ecs/schedule/graph/graph.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

class ECS_API ScheduleManager
{
  public:
    ScheduleManager();

  public:
    // 期望有一个配置，可以转换为图的关系，设置到mScheduleGraph。
    void ConfigSchedulePhase()
    {
    }

  protected:
    Graph mScheduleGraph;
};
