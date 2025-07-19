module;

#include "module_export.h"

export module core.memory:malloc;

import core;
import std;

export class CORE_API MallocAllocator
{
    static inline std::thread::id sMainThreadId = std::this_thread::get_id();
    static inline std::atomic<bool> sInitted = false;

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
