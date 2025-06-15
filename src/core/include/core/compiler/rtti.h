#pragma once

#include <core/minimal.h>
#ifdef __clang__
#    include <cxxabi.h>
#endif

/**
 * @brief 仅用于Debug
 */
template <typename T>
inline const char* GetTypeName()
{
    const auto name = typeid( T ).name();
#ifdef __clang__
    return abi::__cxa_demangle( name, nullptr, nullptr, nullptr );
#else
    return name;
#endif
}