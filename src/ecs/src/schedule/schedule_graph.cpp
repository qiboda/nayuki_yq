
#include "ecs/schedule/schedule_graph.h"

ScheduleGraph::ScheduleGraph()
{
}

void ScheduleGraph::AddSystemNodeConfig( class ScheduleSystemNodeConfig&& config )
{
    mSystemNodeConfigs.push_back( std::move( config ) );
}

void ScheduleGraph::AddSystemSetNodeConfig( class ScheduleSystemSetNodeConfig&& config )
{
    mSystemSetNodeConfigs.push_back( std::move( config ) );
}

void ScheduleGraph::ApplyNodeConfigs()
{
    for ( auto&& config : mSystemNodeConfigs )
    {
        config.Apply( mSchedule );
    }

    for ( auto&& config : mSystemSetNodeConfigs )
    {
        config.Apply( mSchedule );
    }
}