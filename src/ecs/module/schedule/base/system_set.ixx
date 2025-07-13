module;

#include "module_export.h"

export module ecs.schedule:system_set;

import std;
import core;
import core.misc;
import core.registry;
import core.concepts.derived;

/**
 * @brief 所有的SystemSet的基类。
 */
export struct ECS_API SystemSetBase
{
};

export template <typename T>
constexpr inline bool IsSystemSet = IsStrictDerivedConcept<SystemSetBase, T>;

export template <typename T>
concept IsSystemSetConcept = IsSystemSet<T>;

export struct ECS_API SystemSetId : public Id
{
};

export ECS_API inline bool operator==( const SystemSetId& lhs, const SystemSetId& rhs )
{
    return lhs.Index() == rhs.Index();
}

export ECS_API inline auto operator<=>( const SystemSetId& lhs, const SystemSetId& rhs )
{
    return lhs.Index() <=> rhs.Index();
}


template <>
struct std::hash<SystemSetId>
{
    usize operator()( const SystemSetId& id ) const
    {
        return std::hash<u32>()( id.Index() );
    }
};

export class ECS_API SystemSetIdRegistry : public IdRegistry<SystemSetId, SystemSetBase>
{
};

export struct ECS_API SystemSetInfo
{
    std::string mName;
    std::string mDescription;
    SystemSetId mId;
};

export class ECS_API SystemSetInfoRegistry : public SystemSetIdRegistry
{
};
