module;

#include <meta_forge/meta_forge.h>

module meta_forge.forge;

import meta_forge.meta_info;
import meta_forge.module_info;
import meta_forge.decl_context_utils;
import meta_forge.data_generator;
import meta_forge.tmpl.render;
import meta_forge.struct_attribute_collector;
import meta_forge.class_attribute_collector;
import meta_forge.command_list_parser;

import std;
import core;
import meta_forge;
import core.fs.paths;
import core.misc.path;

class FilteringCompilationDatabase : public clang::tooling::CompilationDatabase
{
  public:
    FilteringCompilationDatabase( CompilationDatabase& Inner )
        : InnerDB( Inner )
    {
    }

    std::vector<clang::tooling::CompileCommand> getCompileCommands( llvm::StringRef FilePath ) const override
    {
        auto Commands = InnerDB.getCompileCommands( FilePath );

        // mClangCompileCommands[FilePath.str()].resize( Commands.size() );

        for ( usize j = 0; j < Commands.size(); ++j )
        {
            // auto& ClangCommand = mClangCompileCommands[FilePath.str()][j];
            // ClangCommand.clear();
            auto& Command = Commands[j];

            std::vector<std::string> Filtered;
            for ( usize i = 0; i < Command.CommandLine.size(); ++i )
            {
                const auto& Arg = Command.CommandLine[i];
                if ( Arg == "/W4" )
                    continue;
                // 不编译
                if ( Arg == "/c" )
                    continue;
                if ( Arg.find( "/Fobuild" ) != std::string::npos )
                    continue;
                if ( Arg == "/WX" )
                    continue;
                if ( Arg == "/Zc:preprocessor" )
                    continue;
                if ( Arg == "/std:c++latest" )
                {
                    // 可能是clang的c++20
                    // ClangCommand.push_back( "-std=c++20" );
                    continue;
                }
                if ( Arg == "/reference" )
                {
                    // const auto& RefFile = Command.CommandLine[i + 1];
                    // ClangCommand.push_back( "-fmodule-file=" + RefFile );
                    i += 1;
                    continue;
                }
                Filtered.push_back( Arg );
            }
            Command.CommandLine = std::move( Filtered );
        }
        return Commands;
    }

    std::vector<std::string> getAllFiles() const override
    {
        return InnerDB.getAllFiles();
    }

    std::vector<clang::tooling::CompileCommand> getAllCompileCommands() const override
    {
        return InnerDB.getAllCompileCommands(); // 可选：也做过滤
    }

    // mutable std::map<std::string, std::vector<std::vector<std::string>>> mClangCompileCommands;

  private:
    CompilationDatabase& InnerDB;
};

void Forge::Init( usize argc, const char** argv )
{
    this->mArgc = argc;
    this->mArgv = argv;

    mMetaInfoManager = std::make_shared<MetaInfoManager>();
}

void Forge::RunPhase( ForgePhase phase )
{
    mPhase = phase;
    switch ( mPhase )
    {
    case ForgePhase::None:
        break;
    case ForgePhase::LoadCompileArgs:
        LoadCompileArgs();
        break;
    case ForgePhase::ParseCommands:
        RunParseCommands();
        break;
    case ForgePhase::BuildPcmFiles:
        if ( mModuleInfoManager )
        {
            std::string targetName = mCommandListParser->GetTargetName();
            mModuleInfoManager->CacheModuleInfo();
            mModuleInfoManager->BuildPcmFiles( targetName );
        }
        RunPhase( ForgePhase::GenerateCompileCommands );
        break;
    case ForgePhase::GenerateCompileCommands:
        RunGenerateCompileCommands();
        break;
    case ForgePhase::RunTools:
        RunRunTools();
        break;
    case ForgePhase::GenerateData:
        RunGenerateData();
        break;
    case ForgePhase::RenderTemplate:
        RunRenderTemplate();
        break;
    case ForgePhase::Finish:
        break;
    }
}

void Forge::RunParseCommands()
{
    // 定义参数
    i32 argc = static_cast<i32>( mArgc );
    auto commandListParser = CommandListParser::create( argc, mArgv );
    if ( !commandListParser )
    {
        llvm::errs() << commandListParser.takeError();
        return;
    }

    mCommandListParser = &commandListParser.get();

    if ( mModuleInfoManager )
    {
        if ( mModuleInfoManager->TargetCanMeta( mCommandListParser->GetTargetName() ) == false )
        {
            return;
        }
    }
    RunPhase( ForgePhase::BuildPcmFiles );
}

void Forge::LoadCompileArgs()
{
    std::ifstream file( Paths::EngineFolder() / ".nayuki/compile_args.json" );
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    auto moduleInfo = rfl::json::read<ModuleInfo>( content ).value();
    mModuleInfoManager = std::make_shared<ModuleInfoManager>();
    mModuleInfoManager->SetModuleInfo( moduleInfo );

    RunPhase( ForgePhase::ParseCommands );
}

void Forge::RunGenerateCompileCommands()
{
    // auto sourceFiles = mCommandListParser->getSourcePathList();
    // 扫描指定模块的文件。
}

void Forge::RunRunTools()
{
    // auto compilations = FilteringCompilationDatabase( mCommandListParser->getCompilations() );
    // auto sourceFiles = mCommandListParser->getSourcePathList();

    // clang::tooling::ClangTool Tool( compilations, sourceFiles );
    // Tool.appendArgumentsAdjuster(
    //     clang::tooling::getInsertArgumentAdjuster( "-fsyntax-only", clang::tooling::ArgumentInsertPosition::BEGIN )
    //     );
    // Tool.appendArgumentsAdjuster(
    //     getInsertArgumentAdjuster( "-Wno-everything", clang::tooling::ArgumentInsertPosition::END ) );
    // Tool.appendArgumentsAdjuster( getInsertArgumentAdjuster(
    //     {
    //         "-###",
    //         "-v",
    //         // "-L/home/skwy/.xmake/packages/l/libllvm/19.1.7/407a7eaa7d504e6b993031c21199dfbf/lib",
    //     },
    //     //
    //     "-fmodule-file=core=build/.gens/core_tests/windows/x64/release/rules/bmi/cache/interfaces/dba42005/core.ifc"
    //     //   },
    //     clang::tooling::ArgumentInsertPosition::END ) );

    // for ( auto& file : sourceFiles )
    // {
    //     compilations.getCompileCommands( file );
    //     // auto& clangCompileCommands = compilations.mClangCompileCommands[file];
    //     // for ( usize i = 0; i < clangCompileCommands.size(); ++i )
    //     // {
    //     // const auto& command = clangCompileCommands[i];
    //     // for ( usize j = 0; j < command.size(); ++j )
    //     // {
    //     //     const auto& arg = command[j];
    //     //     std::cout << "Command[" << i << "][" << j << "]: " << arg << std::endl;
    //     // }
    //     //     Tool.appendArgumentsAdjuster(
    //     //         clang::tooling::getInsertArgumentAdjuster( command, clang::tooling::ArgumentInsertPosition::BEGIN
    //     //         ) );
    //     // }
    // }
    // // 添加 - isystem 参数
    // clang::tooling::ArgumentsAdjuster AddIsystem =
    //     getInsertArgumentAdjuster( { "-isystem",
    //                                  "/usr/include/c++/15.1.1",
    //                                  "-isystem",
    //                                  "/usr/include/c++/15.1.1/x86_64-pc-linux-gnu",
    //                                  "-isystem",
    //                                  "/usr/include/c++/15.1.1/backward",
    //                                  "-isystem",
    //                                  "/usr/lib/clang/20/include",
    //                                  "-isystem",
    //                                  "/usr/local/include",
    //                                  "-isystem",
    //                                  "/usr/include",
    //                                  "-march=sapphirerapids",
    //                                  //  "-Xclang",
    //                                  //  "-ast-dump",
    //                                  "-ftime-report",
    //                                  //  "-ftime-trace=tract.json",
    //                                  "-fmodules",
    //                                  "-fmodules-cache-path=./modules-cache" },
    //                                clang::tooling::ArgumentInsertPosition::BEGIN );

    // Tool.appendArgumentsAdjuster( AddIsystem );

    // clang::ast_matchers::MatchFinder Finder;
    // StructAttributeCollector StructHandler;
    // Finder.addMatcher( StructMatcher, &StructHandler );
    // ClassAttributeCollector ClassHandler( mMetaInfoManager );
    // Finder.addMatcher( ClassMatcher, &ClassHandler );

    // Tool.setPrintErrorMessage( true ); // 显示错误信息
    // Tool.run( clang::tooling::newFrontendActionFactory( &Finder ).get() );

    // RunPhase( ForgePhase::GenerateData );
}

void Forge::RunGenerateData()
{
    const std::string json_string = rfl::json::write( *mMetaInfoManager, rfl::json::pretty );

    // FsPath moduleAbsFolder = std::filesystem::absolute( mCommandListParser->GetModuleFolder() );
    // FsPath engineFolder = Paths::EngineFolder();

    // FsPath moduleSubFolder = PathHelper::RemoveCommonPrefix( moduleAbsFolder, engineFolder );
    // FsPath generatedPath = engineFolder / "build" / "generated" / moduleSubFolder;

    // std::ofstream file( generatedPath / "data" / "meta_info.json" );
    // file << json_string;
    // file.close();

    // RunPhase( ForgePhase::RenderTemplate );
}

void Forge::RunRenderTemplate()
{
    RunPhase( ForgePhase::Finish );
}
