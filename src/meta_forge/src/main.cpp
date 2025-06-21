#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/AST/AST.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecordLayout.h>

#include <iostream>

clang::ast_matchers::DeclarationMatcher StructMatcher =
    clang::ast_matchers::recordDecl( clang::ast_matchers::isStruct() ).bind( "struct" );

class StructAttributePrinter : public clang::ast_matchers::MatchFinder::MatchCallback
{
  public:
    void run( const clang::ast_matchers::MatchFinder::MatchResult& Result ) override
    {
        const clang::RecordDecl* RD = Result.Nodes.getNodeAs<clang::RecordDecl>( "struct" );
        if ( !RD || !RD->hasAttrs() )
            return;

        clang::SourceManager* SM = Result.SourceManager;
        clang::SourceLocation Loc = RD->getLocation();
        if ( SM->isInSystemHeader( Loc ) )
        {
            return; // 忽略系统头中的匹配
        }

        clang::ASTContext* Context = Result.Context;
        const clang::RawComment* RC = Context->getRawCommentForDeclNoCache( RD );
        if ( RC )
        {
            llvm::outs() << "Comment for " << RD->getNameAsString() << ": "
                         << RC->getRawText( Context->getSourceManager() ) << "\n";
        }

        std::cout << "Struct: " << RD->getNameAsString() << SM->getFilename(Loc).str() << "\n";
        for ( const clang::Attr* attr : RD->attrs() )
        {
            std::cout << "  Attr Kind: " << attr->getSpelling() << "\n";
            auto* annotate = llvm::dyn_cast<clang::AnnotateAttr>( attr );
            if ( annotate )
            {
                std::cout << "  annotation: " << annotate->getAnnotation().str() << std::endl;
            }
            for ( const clang::FieldDecl* field : RD->fields() )
            {
                clang::QualType fieldType = field->getType();
                std::string fieldName = field->getNameAsString();
                std::string typeStr = fieldType.getAsString();

                llvm::outs() << "  Field: " << fieldName << " : " << typeStr << "\n";

                for ( const clang::Attr* fieldAttr : field->attrs() )
                {
                    auto* fieldAnnotate = llvm::dyn_cast<clang::AnnotateAttr>( fieldAttr );
                    if ( fieldAnnotate )
                    {
                        std::cout << "  annotation: " << fieldAnnotate->getAnnotation().str() << std::endl;
                    }
                }
            }
        }
    }
};

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
    llvm::cl::OptionCategory ToolCategory( "attribute-parser options" );
    auto ExpectedParser = clang::tooling::CommonOptionsParser::create( argc, argv, ToolCategory );
    if ( !ExpectedParser )
    {
        llvm::errs() << ExpectedParser.takeError();
        return 0;
    }
    clang::tooling::CommonOptionsParser& OptionsParser = ExpectedParser.get();

    auto compilations = FilteringCompilationDatabase( OptionsParser.getCompilations() );
    auto sourcePathList = OptionsParser.getSourcePathList();
    clang::tooling::ClangTool Tool( compilations, sourcePathList );

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

    for ( auto& file : sourcePathList )
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

    StructAttributePrinter Handler;
    clang::ast_matchers::MatchFinder Finder;
    Finder.addMatcher( StructMatcher, &Handler );

    return Tool.run( clang::tooling::newFrontendActionFactory( &Finder ).get() );
}
