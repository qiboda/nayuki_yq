#pragma once

#include "core/registry/id.h"
#include <core/minimal.h>
#include <ecs/minimal.h>

class ECS_API IFeature
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

    virtual void Build( class Registry* registry )
    {
        NY_STATIC_ASSERT_MSG( false, "You must implement this virtual function" )
        UNUSED_VAR( registry );
    }
};

template <typename T>
concept IsFeature = IsStrictDerivedConcept<IFeature, T>;

struct FeatureId : public Id
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

class ECS_API FeatureRegistry : public IdRegistry<FeatureId, IFeature>
{
};