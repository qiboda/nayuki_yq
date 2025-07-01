
#if defined( __linux__ )

#    include <unistd.h>

module core.platform.exe;

import core;

FsPath PlatformExe::GetExePath()
{
    char result[PATH_MAX];
    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
    if ( count != -1 )
    {
        return std::filesystem::path( std::string( result, static_cast<size_t>( count ) ) );
    }
    return FsPath();
}

#endif