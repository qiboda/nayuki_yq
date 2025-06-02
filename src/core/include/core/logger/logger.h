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
#include "core/logger/category.h"

// 添加这个前置声明，避免 clangd 警告报错
template <typename T>
class Singleton;

// TODO: Output to console or debugger at the same time.
class CORE_API Logger : public Singleton<Logger>
{
  public:
    friend class Singleton<Logger>;

  protected:
    Logger()
        : Singleton<Logger>()
    {
    }

  public:
    // Register and create a logger.
    void RegisterCategory( const LoggerCategory& loggerCategory );

    std::shared_ptr<spdlog::logger> Get( const LoggerCategory& loggerCategory );

  private:
    std::shared_ptr<spdlog::logger> CreateSyncLogger( const LoggerCategory& loggerCategory );
    std::shared_ptr<spdlog::logger> CreateAsyncLogger( const LoggerCategory& loggerCategory );
};
