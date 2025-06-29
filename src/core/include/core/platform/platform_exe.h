#pragma once

#include "core/type_def.h"
#include <module_export.h>

class CORE_API PlatformExe
{
  public:
    PlatformExe();

  public:
    static FsPath GetExePath();
};
