module;

#include "module_export.h"
#include <core/macro/macro.h>
#include <fmt/format.h>

export module ecs:feature;

import :command_forward;

import std;
import core;
import core.misc;
import core.registry;
import core.concepts.derived;

export class ECS_API IFeature
{
  public:
    IFeature();

    virtual ~IFeature()
    {
    }

  public:
    virtual std::vector<IFeature> GetDependencies()
    {
        return {};
    }

    virtual void Build( Registry* registry )
    {
        NY_STATIC_ASSERT_MSG( false, "You must implement this virtual function" );
        UNUSED_VARS( registry );
    }
};

export template <typename T>
concept IsFeature = IsStrictDerivedConcept<IFeature, T>;

export struct ECS_API FeatureId : public Id
{
};

export inline bool operator==( const FeatureId& lhs, const FeatureId& rhs )
{
    return lhs.Index() == rhs.Index();
}

export inline auto operator<=>( const FeatureId& lhs, const FeatureId& rhs )
{
    return lhs.Index() <=> rhs.Index();
}

export template <>
struct ECS_API std::hash<FeatureId>
{
    usize operator()( const FeatureId& id ) const
    {
        return std::hash<u32>()( id.Index() );
    }
};

export class ECS_API FeatureRegistry : public IdRegistry<FeatureId, IFeature>
{
};