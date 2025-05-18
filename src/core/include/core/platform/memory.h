#pragma once

#include "core/type_def.h"
#include "module_export.h"

class CORE_API Memory
{
  public:
    static usize GetCacheLineSize();

    static usize GetPageSize();
};
