#include "core/platform/memory.h"

#if defined( __linux__ )
#    include <unistd.h>

usize Memory::GetCacheLineSize()
{
    return sysconf( _SC_LEVEL1_DCACHE_LINESIZE );
}

usize Memory::GetPageSize()
{
    return sysconf( _SC_PAGESIZE );
}

#endif