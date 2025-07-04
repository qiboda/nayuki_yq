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

export module spdlog;

// namespace spdlog
// {
// export using logger = ::spdlog::logger;
// export using async_logger = ::spdlog::async_logger;
// export using ::spdlog::get;
// export using ::spdlog::sinks_init_list;
// export using ::spdlog::register_logger;
// export using ::spdlog::spdlog_ex;
// export using ::spdlog::init_thread_pool;
// export using ::spdlog::async_overflow_policy;
// export using ::spdlog::thread_pool;

// namespace sinks
// {
// export using ::spdlog::sinks::ansicolor_stdout_sink_mt;
// export using ::spdlog::sinks::basic_file_sink_mt;
// } // namespace sinks
// } // namespace spdlog