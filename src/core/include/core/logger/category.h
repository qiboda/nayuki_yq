#pragma once

#include "core/compiler/diagnostic.h"
#include "core/type_def.h"
#include <functional>
#include <module_export.h>
#include <string>

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
    LoggerCategory( const Name& loggerName, Type loggerType );

  private:
    Name GetName() const;
    Type GetType() const;

  private:
    friend bool operator==( const LoggerCategory& LHS, const LoggerCategory& RHS )
    {
        std::function<void()>();
        return LHS.GetName() == RHS.GetName() && LHS.GetType() == RHS.GetType();
    }

    friend bool operator!=( const LoggerCategory& LHS, const LoggerCategory& RHS )
    {
        return !( LHS == RHS );
    }

  private:
    // #pragma warning(disable : 4251)
    const Name mLoggerName;
    const Type mLoggerType;
    // #pragma warning(default : 4251)
};

SUPPRESS_UNUSED_MACROS_BEGIN

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

SUPPRESS_UNUSED_MACROS_END