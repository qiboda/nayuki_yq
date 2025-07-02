
#include "core/memory/global_new.h"

import core.memory;

void* operator new( std::size_t size ) noexcept( false )
{
    return Memory::Malloc( size );
}

void* operator new[]( std::size_t size ) noexcept( false )
{
    return Memory::Malloc( size );
}

void* operator new( std::size_t size, const std::nothrow_t& tag ) noexcept
{
    ( void )sizeof( tag );
    return Memory::Malloc( size );
}

void* operator new[]( std::size_t size, const std::nothrow_t& tag ) noexcept
{
    ( void )sizeof( tag );
    return Memory::Malloc( size );
}

void* operator new( std::size_t size, std::align_val_t align ) noexcept( false )
{
    return Memory::AlignedAlloc( size, static_cast<size_t>( align ) );
}

void* operator new[]( std::size_t size, std::align_val_t align ) noexcept( false )
{
    return Memory::AlignedAlloc( size, static_cast<size_t>( align ) );
}

void* operator new( std::size_t size, std::align_val_t align, const std::nothrow_t& tag ) noexcept
{
    ( void )sizeof( tag );
    return Memory::AlignedAlloc( size, static_cast<size_t>( align ) );
}

void* operator new[]( std::size_t size, std::align_val_t align, const std::nothrow_t& tag ) noexcept
{
    ( void )sizeof( tag );
    return Memory::AlignedAlloc( size, static_cast<size_t>( align ) );
}

void operator delete( void* p ) noexcept
{
    Memory::Free( p );
}

void operator delete[]( void* p ) noexcept
{
    Memory::Free( p );
}

void operator delete( void* p, std::size_t size ) noexcept
{
    ( void )sizeof( size );
    Memory::Free( p );
}

void operator delete[]( void* p, std::size_t size ) noexcept
{
    ( void )sizeof( size );
    Memory::Free( p );
}

void operator delete( void* p, std::align_val_t align ) noexcept
{
    ( void )sizeof( align );
    Memory::Free( p );
}

void operator delete[]( void* p, std::align_val_t align ) noexcept
{
    ( void )sizeof( align );
    Memory::Free( p );
}

void operator delete( void* p, std::size_t size, std::align_val_t align ) noexcept
{
    ( void )sizeof( size );
    ( void )sizeof( align );
    Memory::Free( p );
}

void operator delete[]( void* p, std::size_t size, std::align_val_t align ) noexcept
{
    ( void )sizeof( size );
    ( void )sizeof( align );
    Memory::Free( p );
}
