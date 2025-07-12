module;

#include "module_export.h"

export module ecs.schedule:node_config_cache;

import :system_node_config;
import :system_set_node_config;
import std;

export struct ECS_API NodeConfigCache
{
    std::vector<ScheduleSystemNodeConfig> mSystemNodeConfigs;
    std::vector<ScheduleSystemSetNodeConfig> mSystemSetNodeConfigs;
};