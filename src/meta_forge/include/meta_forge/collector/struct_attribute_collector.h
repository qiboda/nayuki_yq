#pragma once

#include <meta_forge/meta_forge.h>

inline clang::ast_matchers::DeclarationMatcher StructMatcher =
    clang::ast_matchers::recordDecl( clang::ast_matchers::isStruct(), clang::ast_matchers::isExpansionInMainFile() )
        .bind( "struct" );

class StructAttributeCollector : public clang::ast_matchers::MatchFinder::MatchCallback
{
  public:
    void run( const clang::ast_matchers::MatchFinder::MatchResult& Result ) override
    {
        const clang::RecordDecl* RD = Result.Nodes.getNodeAs<clang::RecordDecl>( "struct" );
        if ( !RD || !RD->hasAttrs() )
            return;

        clang::SourceManager* SM = Result.SourceManager;
        clang::SourceLocation Loc = RD->getLocation();

        llvm::outs() << SM->getFilename( Loc ) << "\n";

        if ( SM->isInSystemHeader( Loc ) )
        {
            return; // 忽略系统头中的匹配
        }

        bool bNayukiAttr = false;
        for ( const clang::Attr* attr : RD->attrs() )
        {
            // clang::SourceLocation MacroLoc = SM->getTopMacroCallerLoc( attr->getLocation() );
            // clang::SourceLocation MacroLocEnd = MacroLoc.getLocWithOffset(10);

            // clang::CharSourceRange range = clang::CharSourceRange::getTokenRange( MacroLoc, MacroLocEnd);
            // clang::StringRef text = clang::Lexer::getSourceText( range, *SM, clang::LangOptions());
            // llvm::outs() << "text: " << text << "\n";

            auto* annotate = llvm::dyn_cast<clang::AnnotateAttr>( attr );
            if ( annotate )
            {
                if ( annotate->getAnnotation().starts_with( "NAYUKI::" ) )
                {
                    // llvm::outs() << "Attr Kind: " << attr->getSpelling() << "\n";
                    llvm::outs() << "annotation: " << annotate->getAnnotation().str() << "\n";
                    bNayukiAttr = true;
                }
            }
        }

        clang::FileID mainFID = SM->getMainFileID();
        const clang::FileEntry* MainFile = SM->getFileEntryForID( mainFID );
        if ( MainFile )
            llvm::outs() << "Main file: " << MainFile->tryGetRealPathName() << "\n";

        if ( bNayukiAttr )
        {
            clang::ASTContext* Context = Result.Context;
            const clang::RawComment* RC = Context->getRawCommentForDeclNoCache( RD );
            if ( RC )
            {
                llvm::outs() << "Comment for " << RD->getNameAsString() << ":\n"
                             << RC->getRawText( Context->getSourceManager() ) << "\n";
            }

            llvm::outs() << "Struct: " << RD->getNameAsString() << SM->getFilename( Loc ).str() << "\n";
            for ( const clang::FieldDecl* field : RD->fields() )
            {
                clang::QualType fieldType = field->getType();
                std::string fieldName = field->getNameAsString();

                std::string typeStr = fieldType.getAsString();

                // 是引用或者指针类型
                bool bPtr = fieldType->isPointerType();
                bool bLVRef = fieldType->isLValueReferenceType();
                bool bRVRef = fieldType->isRValueReferenceType();
                clang::QualType targetType = ( bLVRef || bRVRef || bPtr ) ? fieldType->getPointeeType() : fieldType;

                clang::Qualifiers qualifiers = targetType.getQualifiers();
                bool bConst = qualifiers.hasConst();
                bool bVolatile = qualifiers.hasVolatile();
                bool bRestrict = qualifiers.hasRestrict();

                llvm::outs() << "  Field: " << fieldName << " : " << typeStr << " is ref: " << bLVRef
                             << " is ptr: " << bPtr << " bconst: " << bConst << " bMove: " << bRVRef
                             << " bVolatile: " << bVolatile << " bRestrict: " << bRestrict << "\n";

                for ( const clang::Attr* fieldAttr : field->attrs() )
                {
                    auto* fieldAnnotate = llvm::dyn_cast<clang::AnnotateAttr>( fieldAttr );
                    if ( fieldAnnotate )
                    {
                        llvm::outs() << "  annotation: " << fieldAnnotate->getAnnotation().str() << "\n";
                    }
                }
            }
        }
    }
};
