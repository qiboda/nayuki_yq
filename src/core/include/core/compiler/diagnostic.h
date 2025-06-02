#pragma once

// Suppress -Wunsafe-buffer-usage only on Clang
#if defined( __clang__ )
#    define DO_PRAGMA( X ) _Pragma( #X )
#    define SUPPRESS_WARNING_BEGIN( warning )                                                                          \
        DO_PRAGMA( clang diagnostic push )                                                                             \
        DO_PRAGMA( clang diagnostic ignored warning )

#    define SUPPRESS_WARNING_END DO_PRAGMA( clang diagnostic pop )
#else
// No-op on other compilers
#    define SUPPRESS_WARNING_BEGIN()
#    define SUPPRESS_WARNING_END
#endif

// Suppress -Wunsafe-buffer-usage only on Clang
#if defined( __clang__ )
#    define SUPPRESS_UNSAFE_BUFFER_USAGE_BEGIN SUPPRESS_WARNING_BEGIN( "-Wunsafe-buffer-usage" )
#    define SUPPRESS_UNSAFE_BUFFER_USAGE_END SUPPRESS_WARNING_END
#else
// No-op on other compilers
#    define SUPPRESS_UNSAFE_BUFFER_USAGE_BEGIN
#    define SUPPRESS_UNSAFE_BUFFER_USAGE_END
#endif

#if defined( __clang__ )
#    define SUPPRESS_UNUSED_MACROS_BEGIN SUPPRESS_WARNING_BEGIN( "-Wunused-macros" )
#    define SUPPRESS_UNUSED_MACROS_END SUPPRESS_WARNING_END
#else
// No-op on other compilers
#    define SUPPRESS_UNUSED_MACROS_BEGIN
#    define SUPPRESS_UNUSED_MACROS_END
#endif

// "-Wcast-function-type" 生成宏
#if defined( __clang__ )
#    define SUPPRESS_CAST_FUNCTION_TYPE_BEGIN SUPPRESS_WARNING_BEGIN( "-Wcast-function-type" )
#    define SUPPRESS_CAST_FUNCTION_TYPE_END SUPPRESS_WARNING_END
#else
// No-op on other compilers
#    define SUPPRESS_CAST_FUNCTION_TYPE_BEGIN
#    define SUPPRESS_CAST_FUNCTION_TYPE_END
#endif
