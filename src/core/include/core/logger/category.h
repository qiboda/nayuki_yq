#pragma once

#include "core/compiler/diagnostic.h"

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