#pragma once

#include "ecs/schedule/phase/phase.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

struct ECS_API StartupPhase : public SchedulePhaseBase
{
};

struct ECS_API FirstPhase : public SchedulePhaseBase
{
};

struct ECS_API PreUpdatePhase : public SchedulePhaseBase
{
};

struct ECS_API UpdatePhase : public SchedulePhaseBase
{
};

struct ECS_API PostUpdatePhase : public SchedulePhaseBase
{
};

struct ECS_API LastPhase : public SchedulePhaseBase
{
};