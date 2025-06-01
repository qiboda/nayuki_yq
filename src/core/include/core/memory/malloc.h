#pragma once

#include "core/macro/macro.h"
#include "core/type_def.h"
#include "module_export.h"
#include "core/memory/memory_tracker.h"

#include <cstdlib>
#include <rpmalloc.h>

class CORE_API MallocAllocator
{
    static std::thread::id sMainThreadId;
    static std::atomic<bool> sInitted;

  public:
    static void Init()
    {
        NY_ASSERT( sMainThreadId == std::this_thread::get_id() )

        NY_ASSERT( sInitted == false )
        // 检查是否是主线程
        rpmalloc_initialize();
        sInitted = true;
    }

    static void Shutdown()
    {
        NY_ASSERT( sMainThreadId == std::this_thread::get_id() )

        NY_ASSERT( sInitted == true )
        rpmalloc_finalize();
        sInitted = false;
    }

    static bool IsInitted()
    {
        if ( sMainThreadId == std::this_thread::get_id() )
        {
            return sInitted;
        }
        else
        {
            return IsThreadInitted();
        }
    }

    static void ThreadInit()
    {
        // 检查是否是主线程, 非主线程才能调用
        rpmalloc_thread_initialize();
    }

    static void ThreadShutdown()
    {
        rpmalloc_thread_finalize( true );
    }

    static bool IsThreadInitted()
    {
        return rpmalloc_is_thread_initialized() > 0 ? true : false;
    }

  public:
    static void* Malloc( usize size )
    {
        if ( IsInitted() == false )
        {
            return std::malloc( size );
        }

        if ( size == 0 )
            size = 1; // 避免零大小分配
        void* ptr = rpmalloc( size );
        if ( !ptr )
        {
            NY_ASSERT( ptr )
            return nullptr;
        }

        usize newSize = rpmalloc_usable_size( ptr );
        MemoryTracker::Malloc( newSize );
        return ptr;
    }

    static void* AlignedAlloc( usize size, usize alignment )
    {
        if ( IsInitted() == false )
        {
            auto aligned_size = ( size + alignment - 1 ) & ~( alignment - 1 );
            return std::malloc( aligned_size );
        }

        NY_ASSERT( alignment > 0 && ( alignment & ( alignment - 1 ) ) == 0 )
        if ( size == 0 )
            size = 1; // 避免零大小分配
        void* ptr = rpaligned_alloc( alignment, size );
        if ( !ptr )
        {
            NY_ASSERT( ptr )
            return nullptr;
        }

        usize newSize = rpmalloc_usable_size( ptr );
        MemoryTracker::Malloc( newSize );
        return ptr;
    }

    static void* Realloc( void* ptr, usize size )
    {
        if ( IsInitted() == false )
        {
            return std::realloc( ptr, size );
        }

        usize old_size = rpmalloc_usable_size( ptr );
        MemoryTracker::Free( old_size );

        if ( size == 0 )
            size = 1; // 避免零大小分配
        void* new_ptr = rprealloc( ptr, size );
        if ( !new_ptr )
        {
            NY_ASSERT( new_ptr )
            return nullptr;
        }

        usize newSize = rpmalloc_usable_size( ptr );
        MemoryTracker::Malloc( newSize );
        return new_ptr;
    }

    static void* Calloc( usize num, usize size )
    {
        if ( IsInitted() == false )
        {
            return std::calloc( num, size );
        }

        if ( num == 0 || size == 0 )
            return nullptr;
        void* ptr = rpcalloc( num, size );
        if ( !ptr )
        {
            NY_ASSERT( ptr )
            return nullptr;
        }

        usize newSize = rpmalloc_usable_size( ptr );
        MemoryTracker::Malloc( newSize );
        return ptr;
    }

    static void Free( void* ptr )
    {
        if ( IsInitted() == false )
        {
            return std::free( ptr );
        }

        usize size = rpmalloc_usable_size( ptr );
        MemoryTracker::Free( size );

        if ( ptr )
            rpfree( ptr );
    }
};
