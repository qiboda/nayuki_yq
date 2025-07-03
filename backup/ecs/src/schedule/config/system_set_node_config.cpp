
#include "ecs/schedule/config/system_set_node_config.h"

ScheduleSystemSetNodeConfig& ScheduleSystemSetNodeConfig::Chain()
{
    mChainFunction = []( std::shared_ptr<class ScheduleBase> scheduleBase, ScheduleNodeIdChainType&& Chain )
    {
        scheduleBase->GetScheduleGraph()->ChainInConfig( std::move( Chain ) );
    };

    return *this;
}
