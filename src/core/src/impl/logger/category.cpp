module;

#include <core/macro/macro.h>
#include <cassert>

module core.logger.category;

#pragma region LoggerCategory

import std;
import fmt;

import core.logger.logger;

LoggerCategory::LoggerCategory( const Name& loggerName, Type loggerType )
    : mLoggerName( loggerName )
    , mLoggerType( loggerType )
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

LoggerCategory::Name LoggerCategory::GetName() const
{
    return mLoggerName;
}

LoggerCategory::Type LoggerCategory::GetType() const
{
    return mLoggerType;
}

#pragma endregion LoggerCategory
