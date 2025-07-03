#pragma once

#include "ecs/schedule/phase/phase.h"

struct ECS_API StartupPhase : public PhaseBase
{
};
NAME_FORMATTER( StartupPhase )

struct ECS_API FirstPhase : public PhaseBase
{
};
NAME_FORMATTER( FirstPhase )

struct ECS_API PreUpdatePhase : public PhaseBase
{
};
NAME_FORMATTER( PreUpdatePhase )

struct ECS_API UpdatePhase : public PhaseBase
{
};
NAME_FORMATTER( UpdatePhase )

struct ECS_API PostUpdatePhase : public PhaseBase
{
};
NAME_FORMATTER( PostUpdatePhase )

struct ECS_API LastPhase : public PhaseBase
{
};
NAME_FORMATTER( LastPhase )