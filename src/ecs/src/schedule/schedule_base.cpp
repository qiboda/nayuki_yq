
#include "ecs/schedule/schedule_base.h"
#include "ecs/schedule/schedule_graph.h"

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