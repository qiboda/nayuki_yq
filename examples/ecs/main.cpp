#include "core/macro/macro.h"

import core.type;
import std;
import ecs;

class MyFeature : public IFeature
{
  public:
    virtual void Build( Registry* registry )
    {
        auto scheduleManager = registry->GetScheduleManager();
        UNUSED_VARS( scheduleManager );
        // schedulerManager->(
        //     []( PhaseConfigure& configure )
        //     {
        //         configure.AddSystem<UpdateWindow>();
        //     } );
    }
};

int main( i32 argc, char* argv[] )
{
    UNUSED_VARS( argc );
    UNUSED_VARS( argv );
}
