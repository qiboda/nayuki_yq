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

void ScheduleBase::Initialize()
{
    mScheduleGraph->SetScheduleBase( shared_from_this() );
    mScheduleGraph->Initialize();
}

void ScheduleBase::CleanUp()
{
}
