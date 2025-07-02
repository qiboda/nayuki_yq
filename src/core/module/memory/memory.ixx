module;

#include <module_export.h>

export module core.memory:memory;

import std;
import :malloc;

export class CORE_API Memory
{
  public:
    static void Init()
    {
        MallocAllocator::Init();
    }

    static void Shutdown()
    {
        MallocAllocator::Shutdown();
    }

    static void ThreadInit()
    {
        MallocAllocator::ThreadInit();
    }

    static void ThreadShutdown()
    {
        MallocAllocator::ThreadShutdown();
    }

    static bool IsThreadInitted()
    {
        return MallocAllocator::IsThreadInitted();
    }

    static bool IsInitted()
    {
        return MallocAllocator::IsInitted();
    }

  public:
    static void* Malloc( usize size )
    {
        return MallocAllocator::Malloc( size );
    }

    static void Free( void* ptr )
    {
        MallocAllocator::Free( ptr );
    }

    static void* Realloc( void* ptr, usize size )
    {
        return MallocAllocator::Realloc( ptr, size );
    }

    static void* Calloc( usize num, usize size )
    {
        return MallocAllocator::Calloc( num, size );
    }

    static void* AlignedAlloc( usize size, usize alignment )
    {
        return MallocAllocator::AlignedAlloc( size, alignment );
    }

  public:
    static void Swap( void* dest, void* src, usize size );
};
