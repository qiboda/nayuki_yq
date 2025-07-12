
module ecs.schedule;

import std;

ScheduleSystemSetNodeConfig& ScheduleSystemSetNodeConfig::Chain()
{
    mChainFunction = []( std::shared_ptr<class ScheduleBase> scheduleBase, ScheduleNodeIdChainType&& Chain )
    {
        scheduleBase->GetScheduleGraph()->ChainInConfig( std::move( Chain ) );
    };

    return *this;
}
