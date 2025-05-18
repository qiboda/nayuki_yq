#include "core/platform/memory.h"

#if defined( __APPLE__ )
#    include <sys/sysctl.h>

usize Memory::GetCacheLineSize()
{
    size_t lineSize = 0;
    size_t size = sizeof( lineSize );
    sysctlbyname( "hw.cachelinesize", &lineSize, &size, nullptr, 0 );
    return lineSize;
}

usize Memory::GetPageSize()
{
    return 4096; // fallback
}

#endif