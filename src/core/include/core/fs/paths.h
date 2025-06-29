#pragma once

#include "core/global.h"
#include "core/logger/log.h"
#include "core/type_def.h"
#include <module_export.h>

/**
 * @brief
 * 1. path: 路径，一个绝对或者相对到具体文件的路径
 * 2. folder: 文件夹，一个绝对或者相对到具体文件夹的路径
 * 此外，路径和文件夹都可以是相对路径或者绝对路径。
 */
class CORE_API Paths
{
  public:
    static FsPath EngineFolder()
    {
        return sEngineFolder;
    }

    static FsPath EngineAssetFolder()
    {
        return sEngineFolder / "assets";
    }

    static FsPath EngineConfigFolder()
    {
        return sEngineFolder / "config";
    }

    static FsPath EnginePluginFolder()
    {
        return sEngineFolder / "plugins";
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
                NY_LOG_CRITICAL( LogCore,
                                 "Error: Cannot find engine folder from executable path: {}",
                                 exePath.string() );
                return FsPath();
            }
        }

        return dir;
    }

    static void SetEngineFolder( const FsPath& path )
    {
        sEngineFolder = path;
    }

  protected:
    static FsPath sEngineFolder;
};