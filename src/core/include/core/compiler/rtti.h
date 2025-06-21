#pragma once

#include <core/minimal.h>
#if defined(__clang__) && !defined(_MSC_VER)
#    include <cxxabi.h>
#endif

/**
 * @brief 仅用于Debug
 */
template <typename T>
inline const char* GetTypeName()
{
    const auto name = typeid( T ).name();
#if defined(__clang__) && !defined(_MSC_VER)
    return abi::__cxa_demangle( name, nullptr, nullptr, nullptr );
#else
    return name;
#endif
}