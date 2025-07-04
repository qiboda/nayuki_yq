module;

#if defined( _WIN32 )

#    include <windows.h>
#    include <filesystem>

module core.platform.exe;

import core;
import std;

FsPath PlatformExe::GetExePath()
{
    char path[MAX_PATH];
    GetModuleFileNameA( NULL, path, MAX_PATH );
    return std::filesystem::path( path );
}

#endif