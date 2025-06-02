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
        NY_ASSERT_MSG( false, "You must implement this virtual function" )
        UNUSED_VAR( registry );
    }
};

template <typename T>
concept IsFeature = std::is_base_of_v<IFeature, T>;

struct FeatureId : public Id
{
};

template <>
struct std::hash<FeatureId>
{
    usize operator()( FeatureId& id )
    {
        return std::hash<usize>()( id.Index() );
    }
};

class ECS_API FeatureRegistry : public IdRegistry<FeatureId, IFeature>
{
};