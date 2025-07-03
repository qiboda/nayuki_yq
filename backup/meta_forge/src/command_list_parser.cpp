
#include "meta_forge/command_list_parser.h"

import core.fs.paths;

// 定义参数
static llvm::cl::OptionCategory ToolCategory( "options" );

static llvm::cl::opt<std::string> ModuleFolder( llvm::cl::Positional,
                                                llvm::cl::desc( "<input module folder relative engine root project>" ),
                                                llvm::cl::Required,
                                                llvm::cl::cat( ToolCategory ) );

static llvm::cl::opt<std::string> CompilationsFolder( "compilations",
                                                      llvm::cl::desc( "compile_commands.json folder path" ),
                                                      llvm::cl::Required,
                                                      llvm::cl::cat( ToolCategory ) );

CommandListParser::CommandListParser( int& argc, const char** argv )
{
    auto error = init( argc, argv );
    if ( error )
    {
        llvm::outs() << error << "\n";
        return;
    }

    // 收集文件列表
    SourcePathList.clear();
    mModuleFolder = ModuleFolder.getValue();
    CollectSourceFiles( mModuleFolder );

    mCompilationsFolder = CompilationsFolder.getValue();
    std::string ErrorMsg;
    Compilations = clang::tooling::CompilationDatabase::loadFromDirectory(mCompilationsFolder, ErrorMsg );
    
	llvm::outs() << ErrorMsg << "\n";
}

llvm::Expected<CommandListParser> CommandListParser::create( int& argc, const char** argv )
{
    auto c = CommandListParser( argc, argv );
    return c;
}

// 扫描路径下所有 .cpp/.c/.h/.hpp 文件
void CommandListParser::CollectSourceFiles( const std::string& path )
{
    llvm::sys::fs::file_status status;
    if ( llvm::sys::fs::status( path, status ) )
        return;

    if ( llvm::sys::fs::is_regular_file( status ) )
    {
        if ( llvm::StringRef( path ).ends_with( ".cpp" ) )
        {
            SourcePathList.push_back( path );
        }
    }
    else if ( llvm::sys::fs::is_directory( status ) )
    {
        std::error_code ec;
        for ( llvm::sys::fs::recursive_directory_iterator it( path, ec ), end; it != end && !ec; it.increment( ec ) )
        {
            if ( !ec && llvm::sys::fs::is_regular_file( it->path() ) )
            {
                CollectSourceFiles( it->path() ); // 递归判定扩展名
            }
        }
    }
}

llvm::Error CommandListParser::init( int& argc, const char** argv )
{
    llvm::cl::HideUnrelatedOptions( ToolCategory );
    if ( llvm::cl::ParseCommandLineOptions( argc, argv, "This is a MetaForge Tool!" ) == false )
    {
        llvm::outs() << "ParseCommandLineOptions error" << "\n";
        return llvm::make_error<llvm::StringError>( "fail", llvm::inconvertibleErrorCode() );
    }

    return llvm::ErrorSuccess();
}
