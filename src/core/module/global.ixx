module;

#include <module_export.h>
#include "core/logger/category.h"

export module core.global;

import core.logger.category;

export CORE_API NY_LOG_CATEGORY( LogCore );