module;

#include "module_export.h"

export module ecs.archetype.define;

import core;

export struct ECS_API ArchetypeComponentMemoryInfo
{
    usize mTotalOffset;
    usize mTotalSize;
    usize mComponentSize;

    friend bool operator==( const ArchetypeComponentMemoryInfo& lhs,
                            const ArchetypeComponentMemoryInfo& rhs ) = default;
};
