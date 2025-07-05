#if defined( __linux__ )

module;

#    include <unistd.h>

module core.platform.memory;

import core;

usize PlatformMemory::GetCacheLineSize()
{
    return static_cast<usize>( sysconf( _SC_LEVEL1_DCACHE_LINESIZE ) );
}

usize PlatformMemory::GetPageSize()
{
    return static_cast<usize>( sysconf( _SC_PAGESIZE ) );
}

#endif