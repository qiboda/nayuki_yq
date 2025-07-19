module;

#include "module_export.h"

export module ecs:schedule_phase_config_set;

import :schedule_phase_config;

import std;

export class ECS_API PhaseConfigSet
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