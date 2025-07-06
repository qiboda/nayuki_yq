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

        for ( const auto& require : *ixxFileInfo.mRequires.get() )
        {
            // todo:ixxFileInfo.mSourcePath 应该替换为pcm文件路径
            command.mArguments.value().push_back( "-fmodule-file=" + require.mLogicalName.get() + "=" +
                                                  ixxFileInfo.mSourcePath.get() );
        }

        command.mArguments.value().push_back( ixxFileInfo.mSourcePath.get() );

        compileCommands.push_back( command );
    }
}

void ModuleInfoManager::BuildOnePcmFile( const std::string& logicalName, const FsPath& buildBasePath )
{
    LogicalInfo& logicalInfo = mLogicalInfoMap.find( logicalName )->second;

    for ( const auto& requiredLogicalName : logicalInfo.mRequireLogicalNames )
    {
        BuildOnePcmFile( requiredLogicalName, buildBasePath );
    }

    const auto* targetInfo = GetTargetInfo( logicalInfo.mTargetName );

    // clang++ --precompile -std=c++23 -fmodules -x c++-module -o non_copyable.pcm
    // -I/home/skwy/repos/nayuki_yq/build/generated
    // -fmodule-file=core.misc.singleton=/home/skwy/repos/nayuki_yq/singleton.pcm
    // /home/skwy/repos/nayuki_yq/src/core/module/misc/non_copyable.ixx
    std::vector<std::string> commandArgs;
    commandArgs.push_back( "clang++" );
    commandArgs.push_back( "--precompile" );
    commandArgs.push_back( "-fmodules" );
    commandArgs.push_back( "-x" );
    commandArgs.push_back( "c++-module" );
    commandArgs.insert( commandArgs.end(),
                        targetInfo->mTargetFlags.get().begin(),
                        targetInfo->mTargetFlags.get().end() );
    for ( const auto& requiredLogicalName : logicalInfo.mRequireLogicalNames )
    {
        LogicalInfo& requiredLogicalInfo = mLogicalInfoMap.find( requiredLogicalName )->second;
        std::string requiredFile =
            buildBasePath / requiredLogicalInfo.mTargetName / (requiredLogicalInfo.mLogicalName + ".pcm");
        commandArgs.push_back( "-fmodule-file=" + requiredLogicalName + "=" + requiredFile );
    }
    commandArgs.push_back( "-o" );
    std::string outputFile = buildBasePath / logicalInfo.mTargetName / (logicalInfo.mLogicalName + ".pcm");
    commandArgs.push_back( outputFile );

    commandArgs.push_back( logicalInfo.mSourcePath );

    // 执行编译命令
    NY_LOG_INFO( LogMetaForge, "Building PCM file: {}", outputFile );
    i32 result = std::system( fmt::format( "{}", fmt::join( commandArgs, " " ) ).c_str() );
    if ( result != 0 )
    {
        NY_LOG_ERROR( LogMetaForge, "Failed to build PCM file: {}", logicalInfo.mSourcePath );
        return;
    }
}

void ModuleInfoManager::BuildPcmFiles( const std::string_view targetName )
{
    FsPath buildBasePath = FsPath(mModuleInfo.mProjectPath.value()) / ".nayuki_yq" / "build";

    const TargetInfo* targetInfo = GetTargetInfo( targetName );
    if ( targetInfo == nullptr )
    {
        NY_LOG_ERROR( LogMetaForge, "Target '{}' not found in module info.", targetName );
        return;
    }

    for ( const auto& ixxFileInfo : targetInfo->mIxxFilesInfos.get() )
    {
        std::string logicalName = ixxFileInfo.mLogicalName.get();
        BuildOnePcmFile( logicalName, buildBasePath );
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
            for ( const auto& require : *ixxFileInfo.mRequires.value() )
            {
                logicalInfo.mRequireLogicalNames.push_back( require.mLogicalName.get() );
            }
            mLogicalInfoMap.emplace( ixxFileInfo.mLogicalName.get(), logicalInfo );
        }
    }
}