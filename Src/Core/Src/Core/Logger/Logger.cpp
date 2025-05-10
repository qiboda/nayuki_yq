// --------------------------------------------------------------------
// Copyright(c) 2019 qibla
// Licensed under the MIT License <http://opensource.org/licenses/MIT>.
// See the LICENSE file in the repo root for full license information.
// --------------------------------------------------------------------

#include <Core/Core.h>
#include <iostream> // IWYU pragma: keep

#pragma region LoggerCategory

LoggerCategory::LoggerCategory( const Name &loggerName, Type loggerType )
    : mLoggerName( loggerName ), mLoggerType( loggerType )
{
    if ( mLoggerType == Type::Sync )
    {
        NY_ASSERT_MSG( mLoggerName.substr( 0, 3 ) == "Log", "Sync Logger Name prefix must be 'Log'!" )
    }
    else if ( mLoggerType == Type::Async )
    {
        NY_ASSERT_MSG( mLoggerName.substr( 0, 4 ) == "ALog", "Async Logger Name prefix must be 'ALog'!" )
    }
    Logger::GetInstance().RegisterCategory( *this );
}

LoggerCategory::Name LoggerCategory::GetName() const { return mLoggerName; }

LoggerCategory::Type LoggerCategory::GetType() const { return mLoggerType; }

#pragma endregion LoggerCategory

#pragma region Logger

std::shared_ptr<spdlog::logger> Logger::Get( const LoggerCategory &loggerCategory )
{
    return spdlog::get( loggerCategory.GetName() );
}

void Logger::RegisterCategory( const LoggerCategory &loggerCategory )
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

std::shared_ptr<spdlog::logger> Logger::CreateSyncLogger( const LoggerCategory &loggerCategory )
{
    try
    {
        auto console_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
        if ( console_sink )
        {
            console_sink->set_level( spdlog::level::warn );
        }

        auto file_sink =
            std::make_shared<spdlog::sinks::basic_file_sink_mt>( "./" + loggerCategory.GetName() + ".log", true );
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
    catch ( spdlog::spdlog_ex &ex )
    {
        std::cerr << "spdlog initialization failed: " << ex.what() << std::endl;
        exit( EXIT_FAILURE );
    }
}

std::shared_ptr<spdlog::logger> Logger::CreateAsyncLogger( const LoggerCategory &loggerCategory )
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
        auto asyncLogger = std::make_shared<spdlog::async_logger>(
            loggerCategory.GetName(), sinksInitList, spdlog::thread_pool(), spdlog::async_overflow_policy::block );
        if ( asyncLogger )
        {
            asyncLogger->set_level( spdlog::level::trace );
            spdlog::register_logger( asyncLogger );
        }
        return asyncLogger;
    }
    catch ( spdlog::spdlog_ex &ex )
    {
        std::cerr << "spdlog initialization failed: " << ex.what() << std::endl;
        exit( EXIT_FAILURE );
    }
}

#pragma endregion Logger
