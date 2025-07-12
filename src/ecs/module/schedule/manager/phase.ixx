module;

#include "module_export.h"
#include "core/macro/formatter.h"
#include "core/macro/macro.h"
#include <fmt/format.h>

export module ecs.schedule.manager:phase;

import core.container.small_vector;
import core.misc;
import core.concepts.derived;
import core.registry;
import core;

export struct ECS_API PhaseBase
{
};
NAME_FORMATTER( PhaseBase )

export template <typename T>
concept IsSchedulePhase = IsStrictDerivedConcept<PhaseBase, T>;

export struct ECS_API PhaseId : public Id
{
    friend auto operator<=>( const PhaseId& lhs, const PhaseId& rhs )
    {
        return lhs.mId <=> rhs.mId;
    }
};

template <>
struct std::hash<PhaseId>
{
    usize operator()( const PhaseId& id ) const
    {
        return std::hash<u32>()( id.Index() );
    }
};

export class ECS_API PhaseIdRegistry : public IdRegistry<PhaseId, PhaseBase>
{
};

export struct ECS_API PhaseInfo
{
    PhaseId mId;
    std::string mName;
};

export using PhaseIdChainType = SmallVector<PhaseId, 8>;
