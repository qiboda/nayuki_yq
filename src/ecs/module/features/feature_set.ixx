module;

#include "module_export.h"

export module ecs.features.set;

import ecs.features.feature;

export class ECS_API IFeatureSet : public IFeature
{
  public:
    IFeatureSet();
};