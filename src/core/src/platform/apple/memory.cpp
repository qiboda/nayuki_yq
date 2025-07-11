
#if defined( __APPLE__ )

module;

#    include <sys/sysctl.h>

module core.platform.memory;

usize PlatformMemory::GetCacheLineSize()
{
    size_t lineSize = 0;
    size_t size = sizeof( lineSize );
    sysctlbyname( "hw.cachelinesize", &lineSize, &size, nullptr, 0 );
    return lineSize;
}

usize PlatformMemory::GetPageSize()
{
    return 4096; // fallback
}

#endif