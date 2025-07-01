#pragma once

#include "core/compiler/diagnostic.h"

#pragma region HelperMacro

SUPPRESS_UNUSED_MACROS_BEGIN

namespace LoggerDetail
{
#ifndef NY_LOG
#    define NY_LOG( LogCategory, LogLevel, LogInfo, ... )                                                              \
        Logger::GetInstance().Get( LogCategory )->LogLevel( LogInfo, ##__VA_ARGS__ )
#endif // !NY_LOG
} // namespace LoggerDetail

#ifndef NY_LOG_INFO
#    define NY_LOG_INFO( LogCategory, LogInfo, ... ) NY_LOG( LogCategory, info, LogInfo, ##__VA_ARGS__ )
#endif // !NY_LOG_INFO

#ifndef NY_LOG_TRACE
#    define NY_LOG_TRACE( LogCategory, LogInfo, ... ) NY_LOG( LogCategory, trace, LogInfo, ##__VA_ARGS__ )
#endif // !NY_LOG_TRACE

#ifndef NY_LOG_DEBUG
#    define NY_LOG_DEBUG( LogCategory, LogInfo, ... ) NY_LOG( LogCategory, debug, LogInfo, ##__VA_ARGS__ )
#endif // !NY_LOG_DEBUG

#ifndef NY_LOG_WARN
#    define NY_LOG_WARN( LogCategory, LogInfo, ... ) NY_LOG( LogCategory, warn, LogInfo, ##__VA_ARGS__ )
#endif // !NY_LOG_WARN

#ifndef NY_LOG_ERROR
#    define NY_LOG_ERROR( LogCategory, LogInfo, ... ) NY_LOG( LogCategory, error, LogInfo, ##__VA_ARGS__ )
#endif // !NY_LOG_ERROR

#ifndef NY_LOG_CRITICAL
#    define NY_LOG_CRITICAL( LogCategory, LogInfo, ... )                                                               \
        NY_LOG( LogCategory, critical, LogInfo, ##__VA_ARGS__ );                                                       \
        NY_STATIC_ASSERT_MSG( false, LogInfo, ##__VA_ARGS__ );
#endif // !NY_LOG_CRITICAL

SUPPRESS_UNUSED_MACROS_END

#pragma endregion HelperMacro