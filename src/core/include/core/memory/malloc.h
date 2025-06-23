#pragma once

#include "core/type_def.h"
#include "module_export.h"

#include <thread>

class CORE_API MallocAllocator
{
    static std::thread::id sMainThreadId;
    static std::atomic<bool> sInitted;

  public:
    static void Init();

    static void Shutdown();

    static bool IsInitted();

    static void ThreadInit();

    static void ThreadShutdown();

    static bool IsThreadInitted();

  public:
    static void* Malloc( usize size );

    static void* AlignedAlloc( usize size, usize alignment );

    static void* Realloc( void* ptr, usize size );

    static void* Calloc( usize num, usize size );

    static void Free( void* ptr );
};
