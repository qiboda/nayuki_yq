#pragma once

import core;

struct ECS_API ArchetypeComponentMemoryInfo
{
    usize mTotalOffset;
    usize mTotalSize;
    usize mComponentSize;

    friend bool operator==( const ArchetypeComponentMemoryInfo& lhs,
                            const ArchetypeComponentMemoryInfo& rhs ) = default;
};
