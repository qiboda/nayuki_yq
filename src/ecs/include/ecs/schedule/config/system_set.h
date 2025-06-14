#pragma once

#include "core/registry/id.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

/**
 * @brief 所有的SystemSet的基类。
 * SystemSetBase的子类不需要定义。只需要声明即可。
 */
struct ECS_API SystemSetBase
{
};

template <typename T>
constexpr inline bool IsSystemSet = IsStrictDerivedConcept<SystemSetBase, T>;

template <typename T>
concept IsSystemSetConcept = IsSystemSet<T>;

struct ECS_API SystemSetId : public Id
{
    friend bool operator==( const SystemSetId& lhs, const SystemSetId& rhs ) = default;
};

template <>
struct std::hash<SystemSetId>
{
    usize operator()( const SystemSetId& id ) const
    {
        return std::hash<u32>()( id.Index() );
    }
};

class ECS_API SystemSetIdRegistry : public IdRegistry<SystemSetId, SystemSetBase>
{
};

struct ECS_API SystemSetInfo
{
    std::string mName;
    std::string mDescription;
    SystemSetId mId;
};

class ECS_API SystemSetInfoRegistry : public SystemSetIdRegistry
{
};
