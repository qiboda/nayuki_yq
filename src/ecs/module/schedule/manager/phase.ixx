module;

#include "module_export.h"
#include "core/macro/formatter.h"
#include "core/macro/macro.h"
#include <fmt/format.h>

export module ecs:schedule_phase;

import core.container.small_vector;
import core.misc;
import core.concepts.derived;
import core.registry;
import core;

export enum class ECS_API PhaseUpdateType : u8 {
    Always,
    Once,
};

export struct ECS_API PhaseBase
{
    static PhaseUpdateType GetUpdateType()
    {
        return PhaseUpdateType::Always;
    }
};
NAME_FORMATTER( PhaseBase )

export template <typename T>
concept IsSchedulePhase = IsStrictDerivedConcept<PhaseBase, T>;

export struct ECS_API PhaseId : public Id
{
};

export ECS_API inline auto operator<=>( const PhaseId& lhs, const PhaseId& rhs )
{
    return lhs.Index() <=> rhs.Index();
}

export ECS_API inline bool operator==( const PhaseId& lhs, const PhaseId& rhs )
{
    return lhs.Index() == rhs.Index();
}

export template <>
struct ECS_API std::hash<PhaseId>
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
    PhaseUpdateType mUpdateType = PhaseUpdateType::Always;
    bool mIsUpdated = false;
};

export using PhaseIdChainType = SmallVector<PhaseId, 8>;
