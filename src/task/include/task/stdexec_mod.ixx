module;

#include <exec/static_thread_pool.hpp>

export module stdexec_mod;

export using ::exec::static_thread_pool;
export using ::stdexec::sender;
export using ::stdexec::schedule;
export using ::stdexec::then;
export using ::stdexec::when_all;