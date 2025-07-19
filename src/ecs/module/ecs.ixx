module;

#include "module_export.h"
#include <core/logger/category.h>

export module ecs;

export import :archetype_mod;
export import :command_mod;
export import :component_mod;
export import :entity_mod;
export import :feature_mod;
export import :registry_mod;
export import :schedule_mod;
export import :system_mod;

import core;
import core.logger.category;

export ECS_API NY_LOG_CATEGORY( LogEcs )
