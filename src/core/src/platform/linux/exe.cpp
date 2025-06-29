#if defined( __linux__ )

#    include "core/platform/platform_exe.h"
#    include "core/type_def.h"
#    include <unistd.h>
#    include <limits.h>
#    include <string>
#    include <filesystem>

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