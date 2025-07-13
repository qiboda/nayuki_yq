// --------------------------------------------------------------------
// Copyright(c) 2019 qibla
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// See the LICENSE file in the repo root for full license information.
// --------------------------------------------------------------------
module;

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

#include <core/macro/macro.h>
#include <cstdlib>

module core.logger.logger;

import core.logger.category;
import core.fs.paths;

#pragma region Logger

std::shared_ptr<spdlog::logger> Logger::Get( const LoggerCategory& loggerCategory )
{
    return spdlog::get( loggerCategory.GetName() );
}

void Logger::RegisterCategory( const LoggerCategory& loggerCategory )
{
    switch ( loggerCategory.GetType() )
    {
    case LoggerCategory::Type::Sync:
    {
        if ( Get( loggerCategory ) == nullptr )
        {
            CreateSyncLogger( loggerCategory );
        }
        else
        {
            std::cerr << "sync category already exist this category:  " << loggerCategory.GetName()
                      << ", duplication register!" << std::endl;
        }
        break;
    }
    case LoggerCategory::Type::Async:
    {
        if ( Get( loggerCategory ) == nullptr )
        {
            CreateAsyncLogger( loggerCategory );
        }
        else
        {
            std::cerr << "async category already exist this category:  " << loggerCategory.GetName()
                      << ", duplication register!" << std::endl;
        }
        break;
    }
    }
}

std::shared_ptr<spdlog::logger> Logger::CreateSyncLogger( const LoggerCategory& loggerCategory )
{
    try
    {
        auto console_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
        if ( console_sink )
        {
            console_sink->set_level( spdlog::level::warn );
        }

        // Paths::ManuallyUpdateEngineFolder();
        FsPath savedFolder = Paths::EngineSavedFolder();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>( (savedFolder / "logs" /
                                                                                  ( loggerCategory.GetName() + ".log" )).string(),
                                                                              true );
        if ( file_sink )
        {
            file_sink->set_level( spdlog::level::trace );
        }

        spdlog::sinks_init_list sinksInitList{ console_sink, file_sink };
        auto syncLogger = std::make_shared<spdlog::logger>( loggerCategory.GetName(), sinksInitList );
        if ( syncLogger )
        {
            syncLogger->set_level( spdlog::level::trace );
            spdlog::register_logger( syncLogger );
        }
        return syncLogger;
    }
    catch ( spdlog::spdlog_ex& ex )
    {
        std::cerr << "spdlog initialization failed: " << ex.what() << std::endl;
        exit( EXIT_FAILURE );
    }
}

std::shared_ptr<spdlog::logger> Logger::CreateAsyncLogger( const LoggerCategory& loggerCategory )
{
    try
    {
        auto console_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
        if ( console_sink )
        {
            console_sink->set_level( spdlog::level::warn );
        }

        auto file_sink =
            std::make_shared<spdlog::sinks::basic_file_sink_mt>( "./" + loggerCategory.GetName() + ".alog", true );
        if ( file_sink )
        {
            file_sink->set_level( spdlog::level::trace );
        }

        spdlog::init_thread_pool( 8192, 1 );
        spdlog::sinks_init_list sinksInitList{ console_sink, file_sink };
        auto asyncLogger = std::make_shared<spdlog::async_logger>( loggerCategory.GetName(),
                                                                   sinksInitList,
                                                                   spdlog::thread_pool(),
                                                                   spdlog::async_overflow_policy::block );
        if ( asyncLogger )
        {
            asyncLogger->set_level( spdlog::level::trace );
            spdlog::register_logger( asyncLogger );
        }
        return asyncLogger;
    }
    catch ( spdlog::spdlog_ex& ex )
    {
        std::cerr << "spdlog initialization failed: " << ex.what() << std::endl;
        exit( EXIT_FAILURE );
    }
}

#pragma endregion Logger
