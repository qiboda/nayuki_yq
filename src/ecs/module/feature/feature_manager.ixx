module;

#include "module_export.h"
#include <core/macro/macro.h>

export module ecs:feature_manager;

import std;
import :feature;

export class ECS_API FeatureManager
{
  public:
    FeatureManager();

  public:
    template <IsFeature T>
    void AddedFeature( T&& feature )
    {
        FeatureId id = FeatureRegistry::Get<T>();
        auto ret = mFeatures.emplace( id, std::forward<T>( feature ) );
        NY_ASSERT_MSG( ret.second, "Can not insert same feature!" );
    }

    template <IsFeature T>
    bool ExistFeature()
    {
        FeatureId id = FeatureRegistry::Get<T>();
        auto it = mFeatures.find( id );
        return it != mFeatures.end();
    }

  protected:
    std::unordered_map<FeatureId, IFeature> mFeatures;
};