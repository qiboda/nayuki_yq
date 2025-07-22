module;

#include "module_export.h"

export module ecs.default_feature;

import ecs;
import std;
import core.type;
import ecs.default_phase;

export class ECS_API DefaultPhaseFeature : public IFeature
{
  public:
    DefaultPhaseFeature()
    {
    }

    virtual ~DefaultPhaseFeature() override
    {
    }

  public:
    virtual void Build( Registry* registry ) override
    {
        registry->ConfigurePhase(
            PhaseConfigure::Create<StartupPhase, FirstPhase, PreUpdatePhase, UpdatePhase, PostUpdatePhase, LastPhase>()
                .Chain()
                .Build() );
    }
};