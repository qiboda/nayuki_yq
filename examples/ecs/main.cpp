#include "core/macro/macro.h"
#include <core/logger/log.h>
#include <core/logger/category.h>
#include <fmt/format.h>

import std;
import ecs;
import ecs.default_phase;
import ecs.default_feature;
import core.type;

import core.logger.category;
import core.logger.logger;

NY_LOG_CATEGORY( LogMain );

void Startup()
{
    NY_LOG_INFO( LogMain, "Startup phase running once." );
}

void First()
{
    NY_LOG_INFO( LogMain, "First phase running." );
}

void PreUpdate()
{
    NY_LOG_INFO( LogMain, "Pre update phase running." );
}

void Update()
{
    NY_LOG_INFO( LogMain, "Update phase running." );
}

void UpdatePost()
{
    NY_LOG_INFO( LogMain, "Update post phase running." );
}

void PostUpdate()
{
    NY_LOG_INFO( LogMain, "Post update phase running." );
}

void Last()
{
    NY_LOG_INFO( LogMain, "Last phase running." );
}

class MyFeature : public IFeature
{
  public:
    virtual void Build( Registry* registry )
    {
        registry->AddSystem<StartupPhase>( ScheduleSystemNodeConfig::Create( &Startup ).Build() );
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