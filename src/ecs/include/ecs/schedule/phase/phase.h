#pragma once

#include "core/macro/formatter.h"
#include "core/registry/id.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

struct ECS_API PhaseBase
{
};
NAME_FORMATTER( PhaseBase )

template <typename T>
concept IsSchedulePhase = IsStrictDerivedConcept<PhaseBase, T>;

struct PhaseId : public Id
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

class PhaseIdRegistry : public IdRegistry<PhaseId, PhaseBase>
{
};