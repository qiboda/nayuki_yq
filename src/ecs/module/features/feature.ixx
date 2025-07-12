module;

#include "module_export.h"
#include <core/macro/macro.h>
#include <fmt/format.h>

export module ecs.features.feature;

import std;
import core;
import core.misc;
import core.registry;
import core.concepts.derived;
import ecs.registry.icontext;

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

    virtual void Build( IRegistryContext* registry )
    {
        NY_STATIC_ASSERT_MSG( false, "You must implement this virtual function" );
        UNUSED_VARS( registry );
    }
};

export template <typename T>
concept IsFeature = IsStrictDerivedConcept<IFeature, T>;

export struct FeatureId : public Id
{
};

template <>
struct std::hash<FeatureId>
{
    usize operator()( const FeatureId& id ) const
    {
        return std::hash<u32>()( id.Index() );
    }
};

export class ECS_API FeatureRegistry : public IdRegistry<FeatureId, IFeature>
{
};