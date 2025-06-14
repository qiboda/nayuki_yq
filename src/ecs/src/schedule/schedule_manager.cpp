
#include "ecs/schedule/schedule_manager.h"
#include "ecs/schedule/phase/config.h"

ScheduleManager::ScheduleManager()
{
}

void ScheduleManager::ConfigSchedulePhase( PhaseConfigure&& configure )
{
    mPhaseConfigures.emplace_back( std::move( configure ) );
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
    for ( auto&& configure : mPhaseConfigures )
    {
        configure.Apply( shared_from_this() );
    }
    mPhaseConfigures.clear();
}

ScheduleManager::~ScheduleManager()
{
}
