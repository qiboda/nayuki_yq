
#include "core/memory/malloc.h"

std::thread::id MallocAllocator::sMainThreadId = std::this_thread::get_id();
std::atomic<bool> MallocAllocator::sInitted = false;