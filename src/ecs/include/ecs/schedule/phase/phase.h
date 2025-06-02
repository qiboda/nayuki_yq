#pragma once

#include "core/registry/id.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

struct ECS_API SchedulePhaseBase
{
};

template <>
struct ECS_API std::formatter<SchedulePhaseBase>
{
    // 可选：解析格式字符串（这里只是简单跳过）
    constexpr auto parse( std::format_parse_context& ctx )
    {
        return ctx.begin(); // 不解析任何 format spec
    }

    template <typename TFormatContext>
    auto format( const SchedulePhaseBase& v, TFormatContext& ctx ) const
    {
        UNUSED_VAR( v );
        UNUSED_VAR( ctx );
        return std::format_to( ctx.out(), "SchedulePhaseBase test)" );
    }
};

template <typename T>
concept IsSchedulePhase = IsStrictDerivedConcept<SchedulePhaseBase, T>;

struct SchedulePhaseId : public Id
{
};

class ScheduleIdRegistry : public IdRegistry<SchedulePhaseId, SchedulePhaseBase>
{
};