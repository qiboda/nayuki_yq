#pragma once

#include "ecs/schedule/config/nodes/node.h"
#include "ecs/schedule/config/system_set.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

class ECS_API SystemSetGraph
{
  public:
    SystemSetGraph();

  protected:
    std::unordered_multimap<SystemSetId, ScheduleNodeId> AllNodesInSet;

    std::set<SystemSetId> AllSets;

    std::unordered_map<SystemSetId, SystemSetId> AllEdges;
};