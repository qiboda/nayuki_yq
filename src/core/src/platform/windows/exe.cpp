#if defined( _WIN32 )

#    include "core/platform/platform_exe.h"
#    include "core/type_def.h"
#    include <windows.h>
#    include <iostream>
#    include <filesystem>

FsPath PlatformExe::GetExePath()
{
    char path[MAX_PATH];
    GetModuleFileNameA( NULL, path, MAX_PATH );
    return std::filesystem::path( path );
}

#endif