
#include "ecs/schedule/schedule_manager.h"
#include "ecs/schedule/phase/config.h"
#include "ecs/schedule/phase/config_set.h"

ScheduleManager::ScheduleManager()
    : mPhaseConfigSet( std::make_shared<PhaseConfigSet>() )
{
}

void ScheduleManager::ConfigSchedulePhase( PhaseConfigure&& configure )
{
    mPhaseConfigSet->AddSchedulePhase( std::move( configure ) );
}

void ScheduleManager::ChainInConfig( PhaseIdChainType&& chain )
{
    for ( usize i = 0; i < chain.size() - 1; ++i )
    {
        mScheduleGraph.AddEdge( chain[i], chain[i + 1] );
    }
}

void ScheduleManager::ApplyPhaseConfigures()
{
    mPhaseConfigSet->ApplyPhaseConfigures( shared_from_this() );
}

ScheduleManager::~ScheduleManager()
{
}
