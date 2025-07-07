module;

#include "core/logger/log.h"
#include <core/macro/macro.h>
#include <module_export.h>

export module core.fs.paths;

import core;
import std;
import core.logger.logger;
import core.global;
import fmt;

/**
 * @brief
 * 1. path: 路径，一个绝对或者相对到具体文件的路径
 * 2. folder: 文件夹，一个绝对或者相对到具体文件夹的路径
 * 此外，路径和文件夹都可以是相对路径或者绝对路径。
 */
export class CORE_API Paths
{
  public:
    static FsPath EngineFolder();

    static FsPath EngineAssetFolder()
    {
        return EngineFolder() / "assets";
    }

    static FsPath EngineSavedFolder()
    {
        return EngineFolder() / "saved";
    }

    static FsPath EngineConfigFolder()
    {
        return EngineFolder() / "config";
    }

    static FsPath EnginePluginFolder()
    {
        return EngineFolder() / "plugins";
    }

    /**
     * @brief
     * 当在sEngineFolder 未初始化时使用，可以手动初始化，然后使用其他路径函数。
     */
    static void ManuallyUpdateEngineFolder()
    {
        EngineFolder();
    }

  protected:
    /**
     * @brief 从二进制文件一路向上寻找引擎目录，标记是nayuki.yq文件。
     *  并设置引擎根目录。
     * @param exePath 二进制执行文件路径
     * @return FsPath 引擎根文件夹
     */
    static FsPath ParseEngineFolder( const FsPath& exePath )
    {
        FsPath absPath = std::filesystem::absolute( exePath );
        FsPath dir = absPath.parent_path();
        while ( std::filesystem::exists( dir / "nayuki.yq" ) == false )
        {
            dir = dir.parent_path();
            if ( dir == dir.root_path() )
            {
                return FsPath();
            }
        }

        return dir;
    }
};