module core.memory;

import core.platform.memory;
import core;
import std;

void Memory::Swap( void* dest, void* src, usize size )
{
    u8* temp = PlatformMemory::Alloca<u8>( size );
    std::memcpy( temp, dest, size );
    std::memcpy( dest, src, size );
    std::memcpy( src, temp, size );
}