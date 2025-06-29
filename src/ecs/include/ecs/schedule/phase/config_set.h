#pragma once

#include "ecs/schedule/phase/config.h"

class ECS_API PhaseConfigSet
{
  public:
    PhaseConfigSet();

    void AddSchedulePhase( PhaseConfigure&& configure )
    {
        mPhaseConfigures.push_back( std::move( configure ) );
    }

    void ApplyPhaseConfigures( std::shared_ptr<class ScheduleManager> scheduleManager );

  protected:
    std::vector<PhaseConfigure> mPhaseConfigures;
};