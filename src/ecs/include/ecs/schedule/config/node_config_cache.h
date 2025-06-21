
#pragma once

#include "ecs/schedule/config/system_node_config.h"
#include "ecs/schedule/config/system_set_node_config.h"

struct ECS_API NodeConfigCache
{
    std::vector<ScheduleSystemNodeConfig> mSystemNodeConfigs;
    std::vector<ScheduleSystemSetNodeConfig> mSystemSetNodeConfigs;
};