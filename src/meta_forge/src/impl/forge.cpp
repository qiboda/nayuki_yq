module;

#include <meta_forge/meta_forge.h>
#include <core/logger/log.h>

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
import core.logger.logger;

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
    if ( mModuleInfoManager )
    {
        std::string targetName = mCommandListParser->GetTargetName();
        mModuleInfoManager->GenerateCompileCommands( targetName );
    }

    RunPhase( ForgePhase::RunTools );
}

void Forge::RunRunTools()
{
    if ( mModuleInfoManager == nullptr )
    {
        return;
    }

    const FsPath ccPath = mModuleInfoManager->GetCompileCommandsPath();

    std::string errorMsg;
    std::unique_ptr<clang::tooling::CompilationDatabase> compilations =
        clang::tooling::JSONCompilationDatabase::loadFromFile( ccPath.string(),
                                                               errorMsg,
                                                               clang::tooling::JSONCommandLineSyntax::AutoDetect );
    if ( errorMsg.empty() == false )
    {
        NY_LOG_ERROR( LogMetaForge,
                      "Load compile commands from '{}' fail: {}",
                      ccPath.parent_path().string(),
                      errorMsg );
        return;
    }

    std::string targetName = mCommandListParser->GetTargetName();
    auto ixxFiles = mModuleInfoManager->GetTargetAllIxxFiles( targetName );

    clang::tooling::ClangTool Tool( *compilations, ixxFiles );
    // Tool.appendArgumentsAdjuster(
    //     getInsertArgumentAdjuster( "-Wno-everything", clang::tooling::ArgumentInsertPosition::END ) );
    // Tool.appendArgumentsAdjuster( getInsertArgumentAdjuster(
    //     {
    //         "-###",
    //         "-v",
    //         // "-L/home/skwy/.xmake/packages/l/libllvm/19.1.7/407a7eaa7d504e6b993031c21199dfbf/lib",
    //     },
    //     clang::tooling::ArgumentInsertPosition::END ) );

    // 添加 - isystem 参数
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

    clang::ast_matchers::MatchFinder Finder;
    StructAttributeCollector StructHandler;
    Finder.addMatcher( StructMatcher, &StructHandler );
    ClassAttributeCollector ClassHandler( mMetaInfoManager );
    Finder.addMatcher( ClassMatcher, &ClassHandler );

    Tool.setPrintErrorMessage( true ); // 显示错误信息
    Tool.run( clang::tooling::newFrontendActionFactory( &Finder ).get() );

    RunPhase( ForgePhase::GenerateData );
}

void Forge::RunGenerateData()
{
    const std::string json_string = rfl::json::write( *mMetaInfoManager, rfl::json::pretty );

    std::string targetName = mCommandListParser->GetTargetName();
    FsPath engineFolder = Paths::EngineFolder();
    FsPath generatedPath = engineFolder / ".nayuki" / "generated" / targetName;

    FsPath dataPath = generatedPath / "data";
    NY_LOG_INFO( LogMetaForge, "Generate Data path: {}", dataPath.string() );

    if ( std::filesystem::exists( dataPath ) == false )
    {
        std::filesystem::create_directories( dataPath );
    }

    std::ofstream file( dataPath / "meta_info.json" );
    file << json_string;
    file.close();

    NY_LOG_INFO( LogMetaForge, "Generate Data content: {}", json_string );

    RunPhase( ForgePhase::RenderTemplate );
}

void Forge::RunRenderTemplate()
{
    RunPhase( ForgePhase::Finish );
}
