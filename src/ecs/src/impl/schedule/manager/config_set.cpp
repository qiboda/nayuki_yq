
module ecs;

import std;

PhaseConfigSet::PhaseConfigSet()
{
}

void PhaseConfigSet::ApplyPhaseConfigures( std::shared_ptr<ScheduleManager> scheduleManager )
{
    for ( auto&& configure : mPhaseConfigures )
    {
        configure.Apply( scheduleManager );
    }
    mPhaseConfigures.clear();
}
