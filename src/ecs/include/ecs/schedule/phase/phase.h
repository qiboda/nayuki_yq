#pragma once

#include "core/container/small_vector.h"
#include "core/macro/formatter.h"
#include "core/registry/id.h"
#include "core/core.h"

struct ECS_API PhaseBase
{
};
NAME_FORMATTER( PhaseBase )

template <typename T>
concept IsSchedulePhase = IsStrictDerivedConcept<PhaseBase, T>;

struct ECS_API PhaseId : public Id
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

class ECS_API PhaseIdRegistry : public IdRegistry<PhaseId, PhaseBase>
{
};

struct ECS_API PhaseInfo
{
    PhaseId mId;
    std::string mName;
};

using PhaseIdChainType = SmallVector<PhaseId, 8>;
