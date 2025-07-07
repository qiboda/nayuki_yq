
module core.fs.paths;

import core.platform.exe;

// FsPath Paths::sEngineFolder = ParseEngineFolder( PlatformExe::GetExePath() );

FsPath Paths::EngineFolder()
{
    static FsPath sEngineFolder = ParseEngineFolder( PlatformExe::GetExePath() );
    return sEngineFolder;
}