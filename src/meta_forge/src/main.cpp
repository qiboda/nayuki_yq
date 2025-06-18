#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/AST/AST.h>
#include <clang/ASTMatchers/ASTMatchers.h>

#include <iostream>

// const std::string structString = R"(
//     [[my_attr]] struct MyStruct
// {
//     int a;
// };

// struct [[gnu::annotate( "meta" )]] AnnotatedStruct
// {
//     int b;
// };

// )";

// clang::ast_matchers::StatementMatcher StructMatcher = recordDecl( TagDecl::isStruct() ).bind( "struct" );

// class StructAttributePrinter : public MatchFinder::MatchCallback
// {
//   public:
//     void run( const MatchFinder::MatchResult& Result ) override
//     {
//         const RecordDecl* RD = Result.Nodes.getNodeAs<RecordDecl>( "struct" );
//         if ( !RD || !RD->hasAttrs() )
//             return;

//         std::cout << "Struct: " << RD->getNameAsString() << "\n";
//         for ( const Attr* attr : RD->attrs() )
//         {
//             std::cout << "  Attr Kind: " << attr->getSpelling() << "\n";
//         }
//     }
// };

int main()
{
    // llvm::cl::OptionCategory ToolCategory( "attribute-parser options" );
    // auto ExpectedParser = CommonOptionsParser::create( argc, argv, ToolCategory );
    // if ( !ExpectedParser )
    // {
    //     llvm::errs() << ExpectedParser.takeError();
    //     return 1;
    // }
    // CommonOptionsParser& OptionsParser = ExpectedParser.get();

    // ClangTool Tool( OptionsParser.getCompilations(), OptionsParser.getSourcePathList() );

    // StructAttributePrinter Handler;
    // MatchFinder Finder;
    // Finder.addMatcher( StructMatcher, &Handler );

    // return Tool.run( newFrontendActionFactory( &Finder ).get() );
    return 0;
}
