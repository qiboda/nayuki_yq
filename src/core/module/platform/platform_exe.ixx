module;

#include <module_export.h>

export module core.platform.exe;

import core;

export class CORE_API PlatformExe
{
  public:
    PlatformExe();

  public:
    static FsPath GetExePath();
};
