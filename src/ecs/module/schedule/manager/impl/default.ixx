module;

#include "module_export.h"
#include <core/macro/macro.h>
#include <core/macro/formatter.h>
#include <fmt/format.h>

export module ecs.default_phase;

import ecs;

export struct ECS_API StartupPhase : public PhaseBase
{
};
NAME_FORMATTER( StartupPhase )

export struct ECS_API FirstPhase : public PhaseBase
{
};
NAME_FORMATTER( FirstPhase )

export struct ECS_API PreUpdatePhase : public PhaseBase
{
};
NAME_FORMATTER( PreUpdatePhase )

export struct ECS_API UpdatePhase : public PhaseBase
{
};
NAME_FORMATTER( UpdatePhase )

export struct ECS_API PostUpdatePhase : public PhaseBase
{
};
NAME_FORMATTER( PostUpdatePhase )

export struct ECS_API LastPhase : public PhaseBase
{
};
NAME_FORMATTER( LastPhase )
