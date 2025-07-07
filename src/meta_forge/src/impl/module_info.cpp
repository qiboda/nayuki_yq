module;

#include <meta_forge/meta_forge.h>
#include <core/logger/log.h>
#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

module meta_forge.module_info;

import std;
import core;
import meta_forge;
import core.logger.logger;

struct CompileCommand
{
    rfl::Field<"directory", std::string> mDirectory;
    rfl::Field<"arguments", std::vector<std::string>> mArguments;
    rfl::Field<"file", std::string> mFile;
};

struct PcmCommands
{
    std::vector<std::string> mCommands;
    FsPath mBuildBasePath;
    FsPath mProjectPath;
    FsPath mOutputFile;

    PcmCommands( FsPath buildBasePath, FsPath projectPath )
    {
        mBuildBasePath = buildBasePath;
        mProjectPath = projectPath;

        mCommands.push_back( "clang++" );
        mCommands.push_back( "--precompile" );
        mCommands.push_back( "-fmodules" );
        mCommands.push_back( std::string( "-fmodules-cache-path=" ) + ( buildBasePath / "modules_cache" ).string() );

        mCommands.push_back( "-working-directory" );
        mCommands.push_back( projectPath.string() );

        // 避免隐式的重复构建标准库中的模块
        // mCommands.push_back( "-fno-implicit-modules" );

        mCommands.push_back( "-x" );
        mCommands.push_back( "c++-module" );
    }

    void AddTargetFlags( const TargetInfo* targetInfo )
    {
        mCommands.insert( mCommands.end(),
                          targetInfo->mTargetFlags.get().begin(),
                          targetInfo->mTargetFlags.get().end() );
    }

    void AddFmoduleFiles( const std::vector<std::string>& allRequiredLogicalNames,
                          const std::map<std::string, LogicalInfo>& logicalInfoMap )
    {
        for ( const auto& requiredLogicalName : allRequiredLogicalNames )
        {
            if ( requiredLogicalName == "std" )
            {
                std::string requiredFile = mBuildBasePath / "std.pcm";
                mCommands.push_back( "-fmodule-file=std=" + requiredFile );
            }
            else
            {
                const LogicalInfo& requiredLogicalInfo = logicalInfoMap.find( requiredLogicalName )->second;
                std::string requiredFile =
                    mBuildBasePath / requiredLogicalInfo.mTargetName / ( requiredLogicalInfo.mLogicalName + ".pcm" );
                mCommands.push_back( "-fmodule-file=" + requiredLogicalName + "=" + requiredFile );
            }
        }
    }

    void SetOutputFile( const LogicalInfo& logicalInfo )
    {
        FsPath outputFolder = mBuildBasePath / logicalInfo.mTargetName;
        mOutputFile = outputFolder / ( logicalInfo.mLogicalName + ".pcm" );

        mCommands.push_back( std::string( "-fmodule-output=" ) + mOutputFile.string() );
        mCommands.push_back( "-o" );
        mCommands.push_back( mOutputFile );

        mCommands.push_back( mProjectPath / logicalInfo.mSourcePath );
    }

    void SetStdFile()
    {
        mOutputFile = mBuildBasePath / "std.pcm";
        mCommands.push_back( "-Wno-reserved-module-identifier" );
        mCommands.push_back( "-Wno-deprecated-declarations" );
        mCommands.push_back( "-Wno-include-angled-in-module-purview" );

        mCommands.push_back( "-o" );
        mCommands.push_back( mOutputFile.string() );
        mCommands.push_back( std::string( "-fmodule-output=" ) + mOutputFile.string() );
        mCommands.push_back( "/usr/include/c++/15.1.1/bits/std.cc" );
    }

    void Execute()
    {
        if ( std::filesystem::exists( mOutputFile ) == false )
        {
            if ( std::filesystem::exists( mOutputFile.parent_path() ) == false )
            {
                std::filesystem::create_directories( mOutputFile.parent_path() );
            }

            // 执行编译命令
            auto cmd = fmt::format( "{}", fmt::join( mCommands, " " ) );
            NY_LOG_INFO( LogMetaForge, "Building PCM file: {} {}", mOutputFile.string(), cmd );
            i32 result = std::system( cmd.c_str() );
            if ( result != 0 )
            {
                NY_LOG_ERROR( LogMetaForge, "Failed to build PCM file: {}", mOutputFile.string() );
            }
        }
    }
};

void ModuleInfoManager::GenerateCompileCommands( const std::string_view targetName )
{
    std::string compileCommandsFile =
        FsPath( mModuleInfo.mProjectPath.get() ) / ".nayuki_yq" / "generated" / targetName / "compile_commands.json";

    const TargetInfo* targetInfo = GetTargetInfo( targetName );
    if ( targetInfo == nullptr )
    {
        NY_LOG_ERROR( LogMetaForge, "Target '{}' not found in module info.", targetName );
        return;
    }

    std::vector<CompileCommand> compileCommands;
    for ( const auto& ixxFileInfo : targetInfo->mIxxFilesInfos.get() )
    {
        CompileCommand command = { .mDirectory = mModuleInfo.mProjectPath.get(),
                                   .mArguments = std::vector<std::string>(),
                                   .mFile = ixxFileInfo.mSourcePath.get() };

        // Add the compile arguments
        command.mArguments.value().push_back( "clang" );
        command.mArguments.value().push_back( "-c1" );
        command.mArguments.value().push_back( "-fsyntax-only" );
        command.mArguments.value().insert( command.mArguments.value().end(),
                                           targetInfo->mTargetFlags.get().begin(),
                                           targetInfo->mTargetFlags.get().end() );

        std::vector<std::string> outRequiredLogicalNames;
        GetAllRequiredLogicalNames( ixxFileInfo.mLogicalName.get(), outRequiredLogicalNames );
        for ( const auto& logicalName : outRequiredLogicalNames )
        {
            // todo:ixxFileInfo.mSourcePath 应该替换为pcm文件路径
            auto it = mLogicalInfoMap.find( logicalName );
            if ( it->second.mLogicalName == "std" )
            {
                command.mArguments.value().push_back( "-fmodule-file=" + logicalName + "=" +
                                                      ( mBuildBasePath / "std.pcm" ).string() );
            }
            else
            {
                command.mArguments.value().push_back( "-fmodule-file=" + logicalName + "=" +
                                                      ( mBuildBasePath / ( it->second.mLogicalName + ".pcm" ) ).string() );
            }
        }

        command.mArguments.value().push_back( ixxFileInfo.mSourcePath.get() );

        compileCommands.push_back( command );
    }
}

std::vector<std::string> ModuleInfoManager::BuildOnePcmFile( const std::string& logicalName,
                                                             const TargetInfo* targetInfo )
{
    if ( logicalName == "std" )
    {
        PcmCommands pcmCommands( mBuildBasePath, mModuleInfo.mProjectPath.get() );
        pcmCommands.AddTargetFlags( targetInfo );
        pcmCommands.SetStdFile();
        pcmCommands.Execute();
        return { "std" };
    }
    LogicalInfo& logicalInfo = mLogicalInfoMap.find( logicalName )->second;

    std::vector<std::string> allRequiredLogicalNames;
    for ( const auto& requiredLogicalName : logicalInfo.mRequireLogicalNames )
    {
        auto indirect = BuildOnePcmFile( requiredLogicalName, targetInfo );
        allRequiredLogicalNames.insert( allRequiredLogicalNames.end(), indirect.begin(), indirect.end() );
    }

    PcmCommands pcmCommands( mBuildBasePath, mModuleInfo.mProjectPath.get() );
    pcmCommands.AddTargetFlags( targetInfo );

    // 添加直接依赖
    allRequiredLogicalNames.insert( allRequiredLogicalNames.end(),
                                    logicalInfo.mRequireLogicalNames.begin(),
                                    logicalInfo.mRequireLogicalNames.end() );
    pcmCommands.AddFmoduleFiles( allRequiredLogicalNames, mLogicalInfoMap );
    allRequiredLogicalNames.push_back( logicalInfo.mLogicalName );

    pcmCommands.SetOutputFile( logicalInfo );
    pcmCommands.Execute();

    return allRequiredLogicalNames;
}

void ModuleInfoManager::BuildPcmFiles( const std::string_view targetName )
{
    const TargetInfo* targetInfo = GetTargetInfo( targetName );
    if ( targetInfo == nullptr )
    {
        NY_LOG_ERROR( LogMetaForge, "Target '{}' not found in module info.", targetName );
        return;
    }

    for ( const auto& ixxFileInfo : targetInfo->mIxxFilesInfos.get() )
    {
        std::string logicalName = ixxFileInfo.mLogicalName.get();
        BuildOnePcmFile( logicalName, targetInfo );
    }
}

void ModuleInfoManager::GetAllRequiredLogicalNames( const std::string& logicalName,
                                                    std::vector<std::string>& outRequiredLogicalNames ) const
{
    const auto it = mLogicalInfoMap.find( logicalName );
    if ( it != mLogicalInfoMap.end() )
    {
        const auto& requireLogicalNames = it->second.mRequireLogicalNames;
        for ( const auto& requireLogicalName : requireLogicalNames )
        {
            GetAllRequiredLogicalNames( requireLogicalName, outRequiredLogicalNames );
        }
        outRequiredLogicalNames.insert( outRequiredLogicalNames.end(),
                                        requireLogicalNames.begin(),
                                        requireLogicalNames.end() );
    }
}

void ModuleInfoManager::CacheModuleInfo()
{
    for ( const auto& target : mModuleInfo.mTargets.get() )
    {
        for ( const auto& ixxFileInfo : target.mIxxFilesInfos.get() )
        {
            LogicalInfo logicalInfo;
            logicalInfo.mTargetName = target.mTargetName.get();
            logicalInfo.mLogicalName = ixxFileInfo.mLogicalName.get();
            logicalInfo.mSourcePath = ixxFileInfo.mSourcePath.get();
            if ( ixxFileInfo.mRequires.value() )
            {
                for ( const auto& require : *ixxFileInfo.mRequires.value() )
                {
                    logicalInfo.mRequireLogicalNames.push_back( require.mLogicalName.get() );
                }
            }
            mLogicalInfoMap.emplace( ixxFileInfo.mLogicalName.get(), logicalInfo );
        }
    }
}