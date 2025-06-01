#pragma once

#include "core/type_def.h"
#include "module_export.h"

class CORE_API PlatformMemory
{
  public:
    static usize GetCacheLineSize();

    static usize GetPageSize();

    static void* Alloca( usize size );

    template <typename T>
    static T* Alloca( usize size )
    {
        return reinterpret_cast<T*>( Alloca( size ) );
    }
};
