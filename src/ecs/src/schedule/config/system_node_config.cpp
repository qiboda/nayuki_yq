
#include "ecs/schedule/config/system_node_config.h"

ScheduleSystemNodeConfig& ScheduleSystemNodeConfig::Chain()
{
    mChainFunction = []( std::shared_ptr<class ScheduleBase> scheduleBase, ScheduleNodeIdChainType&& Chain )
    {
        scheduleBase->GetScheduleGraph()->ChainInConfig( std::move( Chain ) );
    };

    return *this;
}