#include "core/macro/macro.h"
#include <fmt/format.h>

import std;
import ecs;
import ecs.default_phase;
import ecs.default_feature;
import core.type;

void First()
{
    fmt::print( "first!\n" );
}

void PreUpdate()
{
    fmt::print( "pre update!\n" );
}

void Update()
{
    fmt::print( "update!\n" );
}

void UpdatePost()
{
    fmt::print( "update post!\n" );
}

void PostUpdate()
{
    fmt::print( "post update!\n" );
}

void Last()
{
    fmt::print( "last!\n" );
}

class MyFeature : public IFeature
{
  public:
    virtual void Build( Registry* registry )
    {
        registry->AddSystem<FirstPhase>( ScheduleSystemNodeConfig::Create( &First ).Build() );
        registry->AddSystem<PreUpdatePhase>( ScheduleSystemNodeConfig::Create( &PreUpdate ).Build() );
        registry->AddSystem<UpdatePhase>( ScheduleSystemNodeConfig::Create( &Update ).Before( &UpdatePost ).Build() );
        registry->AddSystem<PostUpdatePhase>( ScheduleSystemNodeConfig::Create( &PostUpdate ).Build() );
        registry->AddSystem<LastPhase>( ScheduleSystemNodeConfig::Create( &Last ).Build() );
    }
};

int main()
{
    Registry registry;
    registry.AddFeature( DefaultPhaseFeature{} );
    registry.AddFeature( MyFeature{} );
    registry.Run();
}