module;

#include <module_export.h>

export module core.logger:category;

import core;
import std;

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
        return LHS.GetName() == RHS.GetName() && LHS.GetType() == RHS.GetType();
    }

  private:
    // #pragma warning(disable : 4251)
    const Name mLoggerName;
    const Type mLoggerType;
    // #pragma warning(default : 4251)
};
