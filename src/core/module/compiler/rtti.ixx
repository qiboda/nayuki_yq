module;

#if defined( __clang__ ) && !defined( _MSC_VER )
#    include <cxxabi.h>
#endif

export module core.compiler.rtti;

import std;

/**
 * @brief 仅用于Debug
 */
export template <typename T>
inline const char* GetTypeName()
{
    const auto name = typeid( T ).name();
#if defined( __clang__ ) && !defined( _MSC_VER )
    return abi::__cxa_demangle( name, nullptr, nullptr, nullptr );
#else
    return name;
#endif
}