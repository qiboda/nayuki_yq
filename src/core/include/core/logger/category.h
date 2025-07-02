#pragma once

#include "core/compiler/diagnostic.h"

SUPPRESS_UNUSED_MACROS_BEGIN

#ifndef NY_LOG_CATEGORY
#    define NY_LOG_CATEGORY( LogCategory ) inline const LoggerCategory LogCategory( #LogCategory, LoggerCategory::Type::Sync );
#endif // !NY_LOG_CATEGORY

#ifndef NY_ALOG_CATEGORY
#    define NY_ALOG_CATEGORY( LogCategory ) inline const LoggerCategory LogCategory( #LogCategory, LoggerCategory::Type::Async );
#endif // !NY_ALOG_CATEGORY

SUPPRESS_UNUSED_MACROS_END