module;

#include <module_export.h>

export module core.platform.memory;

import core;

export class CORE_API PlatformMemory
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
