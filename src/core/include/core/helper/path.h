#pragma once

#include "core/type_def.h"

namespace PathHelper
{
/**
 * @brief a - b
 * 公共前缀部分被去掉后，返回 a 路径剩余的部分。
 */
inline FsPath RemoveCommonPrefix( const FsPath& a, const FsPath& b )
{
    auto aIt = a.begin(), aend = a.end();
    auto bIt = b.begin(), bend = b.end();

    // 找到第一个不相同的位置
    while ( aIt != aend && bIt != bend && *aIt == *bIt )
    {
        ++aIt;
        ++bIt;
    }

    // 返回 a 路径去掉公共前缀的部分
    FsPath result;
    for ( ; aIt != aend; ++aIt )
    {
        result /= *aIt;
    }
    return result;
}

} // namespace PathHelper