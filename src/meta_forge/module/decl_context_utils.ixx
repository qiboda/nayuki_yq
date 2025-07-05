module;

#include <module_export.h>
#include <meta_forge/meta_forge.h>

export module meta_forge.decl_context_utils;

import core;
import meta_forge.meta_info;

export class META_FORGE_API DeclContextUtils
{
  public:
    DeclContextUtils();

    static ContextCombinedPath GetContextPath( const clang::RecordDecl* RD )
    {
        std::vector<ContextPath> stack;
        const clang::DeclContext* DC = RD->getDeclContext();

        while ( DC )
        {
            if ( const auto* NS = llvm::dyn_cast<clang::NamespaceDecl>( DC ) )
            {
                ContextPath path;
                path.mName = NS->getNameAsString();
                path.mType = ContextPathType::Namespace;
                stack.push_back( std::move( path ) );
            }
            else if ( const auto* ClassDecl = llvm::dyn_cast<clang::CXXRecordDecl>( DC ) )
            {
                ContextPath path;
                path.mName = ClassDecl->getNameAsString();
                path.mType = ContextPathType::Class;
                stack.push_back( std::move( path ) );
            }
            else if ( const auto* StructDecl = llvm::dyn_cast<clang::RecordDecl>( DC ) )
            {
                ContextPath path;
                path.mName = StructDecl->getNameAsString();
                path.mType = ContextPathType::Struct;
                stack.push_back( std::move( path ) );
            }
            DC = DC->getParent();
        }

        ContextCombinedPath path;
        path.mCombinedPath.reserve( stack.size() );
        for ( auto it = stack.rbegin(); it != stack.rend(); ++it )
        {
            path.mCombinedPath.push_back( *it );
        }

        return path;
    }
};