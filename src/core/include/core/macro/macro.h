#pragma once

#include <cassert>
// 下方 宏中 std::cerr 使用
#include <iostream> // IWYU pragma: keep
#include <format>   // IWYU pragma: keep

#pragma region disable_warning

// to remove unused variable warning
#define UNUSED_VAR( var ) ( ( void )&var )

#pragma endregion disable_warning

#ifndef NY_PRE_CONDITION
#    define NY_PRE_CONDITION( expr, msg, ... )                                                                         \
        if ( bool( expr ) == false )                                                                                   \
        {                                                                                                              \
            std::cerr << std::format( msg, __VA_ARGS__ ) << std::endl;                                                 \
            assert( expr );                                                                                            \
        }
#endif // !NY_PRE_CONDITION

#ifndef NY_POST_CONDITION
#    define NY_POST_CONDITION( expr, msg, ... )                                                                        \
        if ( bool( expr ) == false )                                                                                   \
        {                                                                                                              \
            std::cerr << std::format( msg, __VA_ARGS__ ) << std::endl;                                                 \
            assert( expr );                                                                                            \
        }
#endif // !NY_POST_CONDITION

#ifndef NY_ASSERT
#    define NY_ASSERT( expr )                                                                                          \
        if ( bool( expr ) == false )                                                                                   \
        {                                                                                                              \
            std::cerr << "Check failed: " << #expr << std::endl;                                                       \
            assert( expr );                                                                                            \
        }
#endif // !NY_ASSERT

#ifndef NY_ASSERT_MSG
#    define NY_ASSERT_MSG( expr, msg, ... )                                                                            \
        if ( bool( expr ) == false )                                                                                   \
        {                                                                                                              \
            std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] " << std::format( msg, __VA_ARGS__ ) << std::endl;   \
            assert( expr );                                                                                            \
        }
#endif // !NY_ASSERT_MSG_MSG

#pragma endregion assert

#pragma region memory

#ifndef NY_DELETE
#    define NY_DELETE( pointer )                                                                                       \
        delete pointer;                                                                                                \
        pointer = nullptr
#endif // !NY_DELETE

#ifndef NY_DELETE_ARRAY
#    define NY_DELETE_ARRAY( array_pointer )                                                                           \
        delete[] pointer;                                                                                              \
        pointer = nullptr
#endif // !NY_DELETE_ARRAY

#pragma endregion memory

#pragma region com

#ifndef NY_ASSERT_HR
#    define NY_ASSERT_HR( x ) NY_ASSERT( SUCCEEDED( ( x ) ) )
#endif // NY_ASSERT_MSG_HR

#ifndef NY_ASSERT_HR_MSG
#    define NY_ASSERT_HR_MSG( x, msg, ... ) NY_ASSERT_MSG( SUCCEEDED( ( x ) ), msg, __VA_ARGS__ )
#endif // NY_ASSERT_MSG_HR_MSG

#ifndef NY_RELEASE_COM
#    define NY_RELEASE_COM( x )                                                                                        \
        if ( x )                                                                                                       \
        {                                                                                                              \
            x->Release();                                                                                              \
            x = nullptr;                                                                                               \
        }
#endif // NY_RELEASE_COM

#pragma endregion com

#pragma region NonConstMemFun Make member variable returned to can modify

#ifndef NON_CONST_MEM_FUN
#    define NON_CONST_MEM_FUN( function )                                                                              \
        template <typename... TArgs_>                                                                                  \
        auto function( TArgs_ &&...args ) -> typename std::remove_const_t<decltype( function( args... ) )>             \
        {                                                                                                              \
            return const_cast<decltype( function( args... ) )>(                                                        \
                std::as_const( *this ).function( std::forward<TArgs_>( args )... ) );                                  \
        }
#endif // !NON_CONST_MEM_FUN

#pragma endregion NonConstMemFun

#pragma region stringify

#ifndef STRINGIFY_INNER
#    define STRINGIFY_INNER( Name ) #Name
#endif
#ifndef STRINGIFY
#    define STRINGIFY( Name ) STRINGIFY_INNER( Name )
#endif

#pragma endregion stringify

#pragma region JOIN
// 宏拼接工具
#define JOIN_IMPL( x, y ) x##y
#define JOIN( x, y ) JOIN_IMPL( x, y )

#pragma endregion JOIN
