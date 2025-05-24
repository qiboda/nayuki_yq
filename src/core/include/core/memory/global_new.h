/// NOTE
/// 每个 DLL 必须引入这个头文件，才能使用全局替换的new和delete。
/// 因为这个重定义不能跨越 DLL 模块。
/// 按照项目规范，放置在模块对应的源文件中，例如core模块，就是在core.cpp中。
#pragma once

#include "core/memory/memory.h"

extern void *operator new( std::size_t size ) noexcept( false )
{
    return Memory::Malloc( size );
}

extern void *operator new[]( std::size_t size ) noexcept( false )
{
    return Memory::Malloc( size );
}

extern void *operator new( std::size_t size, const std::nothrow_t &tag ) noexcept
{
    ( void )sizeof( tag );
    return Memory::Malloc( size );
}

extern void *operator new[]( std::size_t size, const std::nothrow_t &tag ) noexcept
{
    ( void )sizeof( tag );
    return Memory::Malloc( size );
}

extern void *operator new( std::size_t size, std::align_val_t align ) noexcept( false )
{
    return Memory::AlignedAlloc( size, static_cast<size_t>( align ) );
}

extern void *operator new[]( std::size_t size, std::align_val_t align ) noexcept( false )
{
    return Memory::AlignedAlloc( size, static_cast<size_t>( align ) );
}

extern void *operator new( std::size_t size, std::align_val_t align, const std::nothrow_t &tag ) noexcept
{
    ( void )sizeof( tag );
    return Memory::AlignedAlloc( size, static_cast<size_t>( align ) );
}

extern void *operator new[]( std::size_t size, std::align_val_t align, const std::nothrow_t &tag ) noexcept
{
    ( void )sizeof( tag );
    return Memory::AlignedAlloc( size, static_cast<size_t>( align ) );
}

extern void operator delete( void *p ) noexcept
{
    Memory::Free( p );
}

extern void operator delete[]( void *p ) noexcept
{
    Memory::Free( p );
}

extern void operator delete( void *p, std::size_t size ) noexcept
{
    ( void )sizeof( size );
    Memory::Free( p );
}

extern void operator delete[]( void *p, std::size_t size ) noexcept
{
    ( void )sizeof( size );
    Memory::Free( p );
}

extern void operator delete( void *p, std::align_val_t align ) noexcept
{
    ( void )sizeof( align );
    Memory::Free( p );
}

extern void operator delete[]( void *p, std::align_val_t align ) noexcept
{
    ( void )sizeof( align );
    Memory::Free( p );
}

extern void operator delete( void *p, std::size_t size, std::align_val_t align ) noexcept
{
    ( void )sizeof( size );
    ( void )sizeof( align );
    Memory::Free( p );
}

extern void operator delete[]( void *p, std::size_t size, std::align_val_t align ) noexcept
{
    ( void )sizeof( size );
    ( void )sizeof( align );
    Memory::Free( p );
}
