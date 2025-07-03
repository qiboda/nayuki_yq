module core.platform.exe;

#include <filesystem>

PlatformExe::PlatformExe() = default;

FsPath PlatformExe::GetExePath()
{
    return FsPath{ std::filesystem::current_path() };
}