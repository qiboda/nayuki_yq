module;

#include "core/macro/macro.h"

module ecs;

import :schedule_graph;
import std;

ScheduleBase::ScheduleBase()
    : mScheduleGraph( std::make_shared<ScheduleGraph>() )
{
}

void ScheduleBase::AddSystemNodeConfig( ScheduleSystemNodeConfig&& config )
{
    mScheduleGraph->AddSystemNodeConfig( std::move( config ) );
}

void ScheduleBase::AddSystemSetNodeConfig( ScheduleSystemSetNodeConfig&& config )
{
    mScheduleGraph->AddSystemSetNodeConfig( std::move( config ) );
}

void ScheduleBase::Init( Registry* registry )
{
    UNUSED_VARS( registry );
    mScheduleGraph->Init( shared_from_this() );
}

void ScheduleBase::BeforeRun( Registry* registry )
{
    mScheduleGraph->BeforeRun( registry );
}

void ScheduleBase::Run( Registry* registry )
{
    mScheduleGraph->Run( registry );
}
