module;

#include <fmt/format.h>
#include <fmt/base.h>

export module fmt;

namespace fmt
{
export using ::fmt::format;
export using ::fmt::format_to;
export using ::fmt::formatter;
} // namespace fmt