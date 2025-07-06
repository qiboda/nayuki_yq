module;

#include <module_export.h>
#include <meta_forge/meta_forge.h>
#include <map>
#include <string>
#include <vector>
#include <optional>
#include <filesystem>

export module meta_forge.module_info;


import core.misc.non_copyable;
import core;

export struct IxxFileRequire
{
    rfl::Field<"logical-name", std::string> mLogicalName;
};

export struct IxxFilesInfo
{
    rfl::Field<"source_path", std::string> mSourcePath;
    rfl::Field<"logical_name", std::string> mLogicalName;
    rfl::Field<"requires", std::optional<std::vector<IxxFileRequire>>> mRequires;
};

export struct TargetInfo
{
    rfl::Field<"path", std::string> mTargetPath;
    rfl::Field<"name", std::string> mTargetName;
    rfl::Field<"flags", std::vector<std::string>> mTargetFlags;
    rfl::Field<"can_meta", bool> mCanMeta;
    rfl::Field<"ixxfiles", std::vector<IxxFilesInfo>> mIxxFilesInfos;
};

export struct ModuleInfo
{
    rfl::Field<"path", std::string> mProjectPath;
    rfl::Field<"targets", std::vector<TargetInfo>> mTargets;
};

export struct LogicalInfo
{
    std::string mTargetName;
    std::string mLogicalName;
    std::string mSourcePath;
    std::vector<std::string> mRequireLogicalNames;
};

export class ModuleInfoManager : public NonCopyable
{
  public:
    ModuleInfoManager()
        : NonCopyable()
        , mModuleInfo{ .mProjectPath = "", .mTargets = std::vector<TargetInfo>() }
    {
    }

    void SetModuleInfo( const ModuleInfo& info )
    {
        mModuleInfo = info;
    }

    const ModuleInfo& GetModuleInfo() const
    {
        return mModuleInfo;
    }

  public:
    bool HasTarget( std::string_view targetName ) const
    {
        for ( const auto& target : mModuleInfo.mTargets.get() )
        {
            if ( target.mTargetName.get() == targetName )
            {
                return true;
            }
        }
        return false;
    }

    bool TargetCanMeta( std::string_view targetName ) const
    {
        for ( const auto& target : mModuleInfo.mTargets.get() )
        {
            if ( target.mTargetName.get() == targetName && target.mCanMeta.get() )
            {
                return true;
            }
        }
        return false;
    }

    const TargetInfo* GetTargetInfo( std::string_view targetName ) const
    {
        for ( const auto& target : mModuleInfo.mTargets.get() )
        {
            if ( target.mTargetName.get() == targetName )
            {
                return &target;
            }
        }
        return nullptr;
    }

    void BuildPcmFiles( const std::string_view targetName );
    void BuildOnePcmFile( const std::string& logicalName, const FsPath& buildBasePath );

    void GenerateCompileCommands( const std::string_view targetName );

    void CacheModuleInfo();

  private:
    ModuleInfo mModuleInfo;

    std::map<std::string, LogicalInfo> mLogicalInfoMap;
};