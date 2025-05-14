/*
 * TODO: add custom format and custom header, add custom thread id?
 *      add a default nayukiyq category to logger.
 *      check log category prefix to log.
 */
#pragma once

#ifdef DEBUG
#    define SPDLOG_DEBUG_ON
#    define SPDLOG_TRACE_ON
#endif // !DEBUG

#ifdef _WIN32
#    ifndef SPDLOG_WCHAR_TO_UTF8_SUPPORT
#        define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#    endif // !SPDLOG_WCHAR_TO_UTF8_SUPPORT
#endif

#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include <spdlog/sinks/ansicolor_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <core/minimal.h>

class CORE_API LoggerCategory
{
  public:
    using Name = std::string;

  public:
    friend class Logger;

  public:
    enum class Type : u8
    {
        Sync,
        Async
    };

  public:
    LoggerCategory( const Name &loggerName, Type loggerType );

  private:
    Name GetName() const;
    Type GetType() const;

  private:
    friend bool operator==( const LoggerCategory &LHS, const LoggerCategory &RHS )
    {
        std::function<void()>();
        return LHS.GetName() == RHS.GetName() && LHS.GetType() == RHS.GetType();
    }

    friend bool operator!=( const LoggerCategory &LHS, const LoggerCategory &RHS )
    {
        return !( LHS == RHS );
    }

  private:
    // #pragma warning(disable : 4251)
    const Name mLoggerName;
    const Type mLoggerType;
    // #pragma warning(default : 4251)
};

// 添加这个前置声明，避免 clangd 警告报错
template <typename T> class Singleton;

// TODO: Output to console or debugger at the same time.
class CORE_API Logger : public Singleton<Logger>
{
  public:
    friend class Singleton<Logger>;

  protected:
    Logger() : Singleton<Logger>()
    {
    }

  public:
    // Register and create a logger.
    void RegisterCategory( const LoggerCategory &loggerCategory );

    std::shared_ptr<spdlog::logger> Get( const LoggerCategory &loggerCategory );

  private:
    std::shared_ptr<spdlog::logger> CreateSyncLogger( const LoggerCategory &loggerCategory );
    std::shared_ptr<spdlog::logger> CreateAsyncLogger( const LoggerCategory &loggerCategory );
};

#pragma region HelperMacro

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-macros"

#ifndef NY_LOG_CATEGORY_DECLARED
#    define NY_LOG_CATEGORY_DECLARED( LogCategory ) extern const LoggerCategory LogCategory;
#endif // !NY_LOG_CATEGORY_DECLARED

#ifndef NY_LOG_CATEGORY_DEFINITION
#    define NY_LOG_CATEGORY_DEFINITION( LogCategory )                                                                  \
        const LoggerCategory LogCategory( #LogCategory, LoggerCategory::Type::Sync );
#endif // !NY_LOG_CATEGORY_DEFINITION

#ifndef NY_ALOG_CATEGORY_DECLARED
#    define NY_ALOG_CATEGORY_DECLARED( LogCategory ) extern const LoggerCategory LogCategory;
#endif // !NY_ALOG_CATEGORY_DECLARED

#ifndef NY_ALOG_CATEGORY_DEFINITION
#    define NY_ALOG_CATEGORY_DEFINITION( LogCategory )                                                                 \
        const LoggerCategory LogCategory( #LogCategory, LoggerCategory::Type::Async );
#endif // !NY_ALOG_CATEGORY_DEFINITION

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
#    define NY_LOG_TRACE( LogCategory, TEXT, ... ) NY_LOG( LogCategory, trace, LogInfo, ##__VA_ARGS__ )
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
#    define NY_LOG_CRITICAL( LogCategory, LogInfo, ... ) NY_LOG( LogCategory, critical, LogInfo, ##__VA_ARGS__ )
#endif // !NY_LOG_CRITICAL

#pragma clang diagnostic pop

#pragma endregion HelperMacro
