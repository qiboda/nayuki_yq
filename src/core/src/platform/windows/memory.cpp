#include "core/platform/memory.h"

#if defined( _WIN32 )
#    include <Windows.h>

usize Memory::GetCacheLineSize()
{
    size_t line_size = 0;
    DWORD buffer_size = 0;
    DWORD i = 0;
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION *buffer = nullptr;

    GetLogicalProcessorInformation( nullptr, &buffer_size );
    buffer = ( SYSTEM_LOGICAL_PROCESSOR_INFORMATION * )malloc( buffer_size );
    GetLogicalProcessorInformation( &buffer[0], &buffer_size );

#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
    for ( i = 0; i <= buffer_size / sizeof( SYSTEM_LOGICAL_PROCESSOR_INFORMATION ); ++i )
    {
        if ( buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 2 )
        {
            line_size = buffer[i].Cache.LineSize;
            break;
        }
    }
#    pragma clang diagnostic pop

    free( buffer );
    return line_size;
}

usize Memory::GetPageSize()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo( &sysInfo );
    return sysInfo.dwPageSize;
}

#endif