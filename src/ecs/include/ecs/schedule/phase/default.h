#pragma once

#include "ecs/schedule/phase/phase.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

struct ECS_API StartupPhase : public SchedulePhase
{
};

struct ECS_API FirstPhase : public SchedulePhase
{
};

struct ECS_API PreUpdatePhase : public SchedulePhase
{
};

struct ECS_API UpdatePhase : public SchedulePhase
{
};

struct ECS_API PostUpdatePhase : public SchedulePhase
{
};

struct ECS_API LastPhase : public SchedulePhase
{
};