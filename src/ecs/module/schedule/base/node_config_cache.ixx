module;

#include "module_export.h"

export module ecs:schedule_node_config_cache;

import :schedule_system_node_config;
import :schedule_system_set_node_config;
import std;

export struct ECS_API NodeConfigCache
{
    std::vector<ScheduleSystemNodeConfig> mSystemNodeConfigs;
    std::vector<ScheduleSystemSetNodeConfig> mSystemSetNodeConfigs;
};