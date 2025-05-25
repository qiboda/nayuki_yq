#pragma once

#include <core/minimal.h>
#include <ecs/minimal.h>

struct ECS_API ArchetypeComponentMemoryInfo
{
    usize mTotalOffset;
    usize mTotalSize;
    usize mComponentSize;
};
