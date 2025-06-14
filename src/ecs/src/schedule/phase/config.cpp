
#include "ecs/schedule/phase/config.h"
#include "ecs/schedule/schedule_manager.h"

void PhaseConfigure::Apply( std::shared_ptr<ScheduleManager> scheduleManager )
{
    ScheduleManager::PhaseIdChainType Chain;
    for ( auto&& func : mCurNodesFunctions )
    {
        PhaseId phaseId = func( scheduleManager );
        Chain.push_back( phaseId );

        for ( auto&& operateFunc : mOperateFunctions )
        {
            operateFunc( scheduleManager, phaseId );
        }
    }

    mChainFunction( scheduleManager, std::move( Chain ) );
}

PhaseConfigure& PhaseConfigure::Chain()
{
    mChainFunction = []( std::shared_ptr<ScheduleManager> scheduleManager, ScheduleManager::PhaseIdChainType&& chain )
    {
        scheduleManager->ChainInConfig( std::move( chain ) );
    };
    return *this;
}