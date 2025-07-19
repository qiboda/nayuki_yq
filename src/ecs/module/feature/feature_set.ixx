module;

#include "module_export.h"

export module ecs:feature_set;

import :feature;

export class ECS_API IFeatureSet : public IFeature
{
  public:
    IFeatureSet();
};