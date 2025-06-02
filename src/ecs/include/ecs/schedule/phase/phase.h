#pragma once

#include "core/registry/id.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

struct ECS_API SchedulePhase
{
  public:
    ~SchedulePhase() = default;
};

template <typename T>
concept IsSchedulePhase = std::is_base_of_v<SchedulePhase, T>;

struct SchedulePhaseId : public Id
{
};

class ScheduleIdRegistry : public IdRegistry<SchedulePhaseId, SchedulePhase>
{
};