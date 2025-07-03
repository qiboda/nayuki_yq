#include "meta_forge/meta_forge.h"
#include "meta_forge/forge_phase.h"
#include <meta_forge/forge.h>

import core;
import core.fs.path;

#include <meta_forge/collector/struct_attribute_collector.h>
#include <meta_forge/collector/class_attribute_collector.h>
#include <meta_forge/command_list_parser.h>

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

        for ( auto& Command : Commands )
        {
            std::vector<std::string> Filtered;
            for ( auto& Arg : Command.CommandLine )
            {
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

  private:
    CompilationDatabase& InnerDB;
};

void Forge::Init( int argc, const char** argv )
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
    case ForgePhase::ParseCommands:
        RunParseCommands();
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
    RunPhase( ForgePhase::RunTools );
}

void Forge::RunRunTools()
{
    // 定义参数
    auto commandListParser = CommandListParser::create( mArgc, mArgv );
    if ( !commandListParser )
    {
        llvm::errs() << commandListParser.takeError();
        return;
    }

    mCommandListParser = &commandListParser.get();

    auto compilations = FilteringCompilationDatabase( mCommandListParser->getCompilations() );
    auto sourceFiles = mCommandListParser->getSourcePathList();

    clang::tooling::ClangTool Tool( compilations, sourceFiles );
    Tool.appendArgumentsAdjuster(
        clang::tooling::getInsertArgumentAdjuster( "-fsyntax-only", clang::tooling::ArgumentInsertPosition::BEGIN ) );
    // Tool.appendArgumentsAdjuster(
    //     getInsertArgumentAdjuster( "-Wno-everything", clang::tooling::ArgumentInsertPosition::END ) );

    // 添加 - isystem 参数
    // clang::tooling::ArgumentsAdjuster AddIsystem = getInsertArgumentAdjuster(
    //     { "-isystem",
    //       "/usr/include/c++/15.1.1",
    //       "-isystem",
    //       "/usr/include/c++/15.1.1/x86_64-pc-linux-gnu",
    //       "-isystem",
    //       "/usr/include/c++/15.1.1/backward",
    //       "-isystem",
    //       "/usr/lib/clang/20/include",
    //       "-isystem",
    //       "/usr/local/include",
    //       "-isystem",
    //       "/usr/include",
    //       "-march=sapphirerapids",
    //       //  "-Xclang",
    //       //  "-ast-dump",
    //       "-ftime-report",
    //       //  "-ftime-trace=tract.json",
    //       "-I/home/skwy/repos/nayuki_yq/build/.objs/meta_forge/linux/x86_64/release/src/meta_forge/include/meta_forge/cxx/",
    //       "-include",
    //       "meta_forge/meta_forge.h",
    //       "-fmodules",
    //       "-fmodules-cache-path=./modules-cache" },
    //     clang::tooling::ArgumentInsertPosition::BEGIN );

    // Tool.appendArgumentsAdjuster( AddIsystem );

    clang::ast_matchers::MatchFinder Finder;
    StructAttributeCollector StructHandler;
    Finder.addMatcher( StructMatcher, &StructHandler );
    ClassAttributeCollector ClassHandler( mMetaInfoManager );
    Finder.addMatcher( ClassMatcher, &ClassHandler );

    Tool.run( clang::tooling::newFrontendActionFactory( &Finder ).get() );

    RunPhase( ForgePhase::GenerateData );
}

void Forge::RunGenerateData()
{
    const std::string json_string = rfl::json::write( *mMetaInfoManager, rfl::json::pretty );

    FsPath moduleAbsFolder = std::filesystem::absolute( mCommandListParser->GetModuleFolder() );
    FsPath engineFolder = Paths::EngineFolder();

    FsPath moduleSubFolder = PathHelper::RemoveCommonPrefix( moduleAbsFolder, engineFolder );
    FsPath generatedPath = engineFolder / "build" / "generated" / moduleSubFolder;

    std::ofstream file( generatedPath / "data" / "meta_info.json" );
    file << json_string;
    file.close();

    RunPhase( ForgePhase::RenderTemplate );
}

void Forge::RunRenderTemplate()
{
    RunPhase( ForgePhase::Finish );
}
