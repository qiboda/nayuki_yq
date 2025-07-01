#pragma once

#include "core/macro/macro.h"
#include <module_export.h>

#include <tbb/concurrent_hash_map.h>

import core;

class CORE_API MemoryTracker
{
    friend fmt::formatter<MemoryTracker>;

  public:
    static void Malloc( usize size )
    {
        GetTotalAllocatedMemoryCount()++;
        GetCurrentAllocatedMemoryCount()++;

        GetTotalMemorySize() += size;
        GetCurrentMemorySize() += size;

        tbb::concurrent_hash_map<usize, usize>::accessor accessor;
        if ( GetMemorySizeCountMap().find( accessor, size ) == false )
        {
            // NY_LOG_WARN( LogCore, "MemoryTracker::Malloc: size {} not found", size );
            GetMemorySizeCountMap().insert( accessor, size );
        }
        accessor->second += 1;
    }

    static void Free( usize size )
    {
        GetTotalFreeMemoryCount()++;
        GetCurrentAllocatedMemoryCount()--;

        GetTotalFreeMemorySize() += size;
        GetCurrentMemorySize() -= size;

        tbb::concurrent_hash_map<usize, usize>::accessor accessor;
        NY_ASSERT( GetMemorySizeCountMap().find( accessor, size ) )
        accessor->second -= 1;
    }

  public:
    // 总计申请内存次数
    static std::atomic<usize>& GetTotalAllocatedMemoryCount()
    {
        static std::atomic<usize> mTotalAllocatedMemoryCount;
        return mTotalAllocatedMemoryCount;
    }

    // 总计释放内存次数
    static std::atomic<usize>& GetTotalFreeMemoryCount()
    {
        static std::atomic<usize> mTotalFreeMemoryCount;
        return mTotalFreeMemoryCount;
    }

    // 当前申请内存次数
    static std::atomic<usize>& GetCurrentAllocatedMemoryCount()
    {
        static std::atomic<usize> mCurrentAllocatedMemoryCount;
        return mCurrentAllocatedMemoryCount;
    }

    // 总计申请内存大小
    static std::atomic<usize>& GetTotalMemorySize()
    {
        static std::atomic<usize> mTotalMemorySize;
        return mTotalMemorySize;
    }

    // 总计释放内存大小
    static std::atomic<usize>& GetTotalFreeMemorySize()
    {
        static std::atomic<usize> mTotalFreeMemorySize;
        return mTotalFreeMemorySize;
    }

    // 当前申请内存大小
    static std::atomic<usize>& GetCurrentMemorySize()
    {
        static std::atomic<usize> mCurrentMemorySize;
        return mCurrentMemorySize;
    }

    // 统计每次内存申请的大小以及次数。
    static tbb::concurrent_hash_map<usize, usize>& GetMemorySizeCountMap()
    {
        static tbb::concurrent_hash_map<usize, usize> mMemorySizeCountMap;
        return mMemorySizeCountMap;
    }

    static std::string ToString()
    {
        return fmt::format(
            "MemoryTracker: TotalAllocatedMemoryCount: {}, TotalFreeMemoryCount: {}, CurrentAllocatedMemoryCount: {}, TotalMemorySize: {}, TotalFreeMemorySize: {}, CurrentMemorySize: {} MemorySizeCountMap: {}",
            MemoryTracker::GetTotalAllocatedMemoryCount().load(),
            MemoryTracker::GetTotalFreeMemoryCount().load(),
            MemoryTracker::GetCurrentAllocatedMemoryCount().load(),
            MemoryTracker::GetTotalMemorySize().load(),
            MemoryTracker::GetTotalFreeMemorySize().load(),
            MemoryTracker::GetCurrentMemorySize().load(),
            MemoryTracker::GetMemorySizeCountMap().size() );
    }
};

// template <>
// struct fmt::formatter<MemoryTracker>
// {
//     constexpr auto parse( fmt::format_parse_context &ctx )
//     {
//         return ctx.begin();
//     }

//     template <typename FormatContext>
//     auto format( const MemoryTracker &value, FormatContext &ctx )
//     {
//     }
// };