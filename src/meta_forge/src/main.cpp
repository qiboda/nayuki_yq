#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/AST/AST.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecordLayout.h>

#include <iostream>
#include <meta_forge/struct_attribute_collector.h>
#include <meta_forge/command_list_parser.h>

clang::ast_matchers::DeclarationMatcher ClassMatcher =
    clang::ast_matchers::recordDecl( clang::ast_matchers::isClass() ).bind( "class" );

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

int main( int argc, const char** argv )
{
    // 定义参数
    auto ExpectedParser = CommandListParser::create( argc, argv );
    if ( !ExpectedParser )
    {
        llvm::errs() << ExpectedParser.takeError();
        return 0;
    }
    CommandListParser& OptionsParser = ExpectedParser.get();

    auto compilations = FilteringCompilationDatabase( OptionsParser.getCompilations() );
    auto sourceFiles = OptionsParser.getSourcePathList();

    // 可选：输出收集到的文件
    for ( auto& f : sourceFiles )
        llvm::outs() << "Found source file: " << f << "\n";

    clang::tooling::ClangTool Tool( compilations, sourceFiles );

    // 添加 -isystem 参数
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
    //                                  //  "-mno-avx512bf16",
    //                                  "-march=sapphirerapids" },
    //                                clang::tooling::ArgumentInsertPosition::BEGIN );

    // Tool.appendArgumentsAdjuster( AddIsystem );

    Tool.appendArgumentsAdjuster(
        clang::tooling::getInsertArgumentAdjuster( "-fsyntax-only", clang::tooling::ArgumentInsertPosition::BEGIN ) );
    // Tool.appendArgumentsAdjuster( getInsertArgumentAdjuster( "-Wno-everything", ArgumentInsertPosition::END ) );

    for ( auto& file : sourceFiles )
    {
        llvm::outs() << "file: " << file << "\n";
        std::vector<clang::tooling::CompileCommand> cmd = compilations.getCompileCommands( file );
        for ( auto& c : cmd )
        {
            for ( auto& cc : c.CommandLine )
            {
                llvm::outs() << cc << " ";
            }
        }
        llvm::outs() << "\n";
    }

    StructAttributeCollector Handler;
    clang::ast_matchers::MatchFinder Finder;
    Finder.addMatcher( StructMatcher, &Handler );

    return Tool.run( clang::tooling::newFrontendActionFactory( &Finder ).get() );
}
