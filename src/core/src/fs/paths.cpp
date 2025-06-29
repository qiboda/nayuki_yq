
#include "core/fs/paths.h"
#include <core/platform/platform_exe.h>

FsPath Paths::sEngineFolder = ParseEngineFolder( PlatformExe::GetExePath() );
