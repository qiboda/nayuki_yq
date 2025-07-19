module;

#include "core/macro/macro.h"
#include <rpmalloc.h>
#include <cstdlib>

module core.memory;

import :memory_tracker;
import std;
import core;

// std::thread::id MallocAllocator::sMainThreadId = std::this_thread::get_id();
// std::atomic<bool> MallocAllocator::sInitted = false;

void MallocAllocator::Init()
{
    NY_ASSERT( sMainThreadId == std::this_thread::get_id() )

    NY_ASSERT( sInitted == false )
    // 检查是否是主线程
    rpmalloc_initialize();
    sInitted = true;
}

void MallocAllocator::Shutdown()
{
    NY_ASSERT( sMainThreadId == std::this_thread::get_id() )

    NY_ASSERT( sInitted == true )
    rpmalloc_finalize();
    sInitted = false;
}

bool MallocAllocator::IsInitted()
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

void MallocAllocator::ThreadInit()
{
    // 检查是否是主线程, 非主线程才能调用
    rpmalloc_thread_initialize();
}

void MallocAllocator::ThreadShutdown()
{
    rpmalloc_thread_finalize( true );
}

bool MallocAllocator::IsThreadInitted()
{
    return rpmalloc_is_thread_initialized() > 0 ? true : false;
}

void* MallocAllocator::Malloc( usize size )
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

void* MallocAllocator::AlignedAlloc( usize size, usize alignment )
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

void* MallocAllocator::Realloc( void* ptr, usize size )
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

void* MallocAllocator::Calloc( usize num, usize size )
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

void MallocAllocator::Free( void* ptr )
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
