#include "core/platform/memory.h"

#if defined( __linux__ )
#    include <unistd.h>

usize PlatformMemory::GetCacheLineSize()
{
    return static_cast<usize>( sysconf( _SC_LEVEL1_DCACHE_LINESIZE ) );
}

usize PlatformMemory::GetPageSize()
{
    return static_cast<usize>( sysconf( _SC_PAGESIZE ) );
}

#endif