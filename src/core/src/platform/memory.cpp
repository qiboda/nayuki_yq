module;

#if defined( _MSC_VER ) // MSVC
#    include <malloc.h>
#elif defined( __GNUC__ ) || defined( __clang__ ) // GCC/Clang
#    include <alloca.h>
#else
#    error "alloca is not supported on this platform"
#endif

module core.platform.memory;

void* PlatformMemory::Alloca( usize size )
{
#if defined( _MSC_VER ) // MSVC
    return _alloca( size );
#elif defined( __GNUC__ ) || defined( __clang__ ) // GCC/Clang
    return alloca( size );
#else
#    error "alloca is not supported on this platform"
#endif
}