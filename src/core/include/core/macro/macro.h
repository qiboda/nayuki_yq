#pragma once

// #include <cstdlib>
#include <cassert>

#pragma region disable_warning

// to remove unused variable warning
#define UNUSED_VARS( ... ) ( ( void )( 0, ##__VA_ARGS__ ) )
#define UNUSED_VARS_PACK( args ) ( ( UNUSED_VARS( args ), ... ) )

#pragma endregion disable_warning

#ifndef NY_PRE_CONDITION
#    define NY_PRE_CONDITION( expr, msg, ... )                                                                         \
        if ( bool( expr ) == false )                                                                                   \
        {                                                                                                              \
            std::cerr << fmt::format( msg, ##__VA_ARGS__ ) << std::endl;                                               \
            assert( expr );                                                                                           \
        }
#endif // !NY_PRE_CONDITION

#ifndef NY_POST_CONDITION
#    define NY_POST_CONDITION( expr, msg, ... )                                                                        \
        if ( bool( expr ) == false )                                                                                   \
        {                                                                                                              \
            std::cerr << fmt::format( msg, ##__VA_ARGS__ ) << std::endl;                                               \
            assert( expr );                                                                                           \
        }
#endif // !NY_POST_CONDITION

#ifndef NY_ASSERT
#    define NY_ASSERT( expr )                                                                                          \
        if ( bool( expr ) == false )                                                                                   \
        {                                                                                                              \
            std::cerr << "Check failed: " << #expr << std::endl;                                                       \
            assert( expr );                                                                                           \
        }
#endif // !NY_ASSERT

// 因为#__VA_ARGS__在没有传入参数时，是一个空字符串，sizeof为1
#ifndef NY_ASSERT_MSG
#    define NY_ASSERT_MSG( expr, msg, ... )                                                                            \
        if ( bool( expr ) == false )                                                                                   \
        {                                                                                                              \
            std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] " << fmt::format( msg, ##__VA_ARGS__ ) << std::endl; \
            assert( expr );                                                                                           \
        }
#endif // !NY_ASSERT_MSG_MSG

#ifndef NY_STATIC_ASSERT_MSG
#    define NY_STATIC_ASSERT_MSG( expr, msg, ... )                                                                     \
        if constexpr ( bool( expr ) == false )                                                                         \
        {                                                                                                              \
            std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] " << fmt::format( msg, ##__VA_ARGS__ ) << std::endl; \
            assert( expr );                                                                                           \
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

#pragma region stringify

#ifndef NY_STRINGIFY_INNER
#    define NY_STRINGIFY_INNER( Name ) #Name
#endif
#ifndef NY_STRINGIFY
#    define NY_STRINGIFY( Name ) NY_STRINGIFY_INNER( Name )
#endif

#pragma endregion stringify

#pragma region NY_JOIN
// 宏拼接工具
#define NY_JOIN_IMPL( x, y ) x##y
#define NY_JOIN( x, y ) NY_JOIN_IMPL( x, y )

#pragma endregion NY_JOIN
