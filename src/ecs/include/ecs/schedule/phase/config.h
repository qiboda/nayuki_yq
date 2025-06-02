#pragma once

#include "ecs/schedule/phase/phase.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

struct Constraint
{
    // before
    // after
    // 排序。
};

struct SchedulePhaseConfig
{
  public:
    std::vector<std::shared_ptr<SchedulePhase>> mPhases;
    std::vector<Constraint> mConstraints;
};
