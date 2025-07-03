#pragma once

#include <meta_forge/meta_forge.h>
#include <meta_forge/meta_info.h>
#include <meta_forge/decl_context_utils.h>

inline clang::ast_matchers::DeclarationMatcher ClassMatcher =
    clang::ast_matchers::recordDecl( clang::ast_matchers::isClass(), clang::ast_matchers::isExpansionInMainFile() )
        .bind( "class" );

class ClassAttributeCollector : public clang::ast_matchers::MatchFinder::MatchCallback
{
  public:
    ClassAttributeCollector( std::shared_ptr<MetaInfoManager> metaInfoManager )
        : MetaInfoManager( metaInfoManager )
    {
    }

  public:
    void run( const clang::ast_matchers::MatchFinder::MatchResult& Result ) override
    {
        const clang::CXXRecordDecl* RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>( "class" );
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
            auto* annotate = llvm::dyn_cast<clang::AnnotateAttr>( attr );
            if ( annotate )
            {
                if ( annotate->getAnnotation().starts_with( "NAYUKI::" ) )
                {
                    // llvm::outs() << "annotation: " << annotate->getAnnotation().str() << "\n";
                    bNayukiAttr = true;

                    // TODO: 检测是否使用了正确的宏。
                }
            }
        }

        if ( bNayukiAttr == false )
        {
            return;
        }
        auto filename = SM->getFilename( Loc );
        auto variableName = RD->getNameAsString();
        auto path = DeclContextUtils::GetContextPath( RD );

        auto uniqueKey = MetaInfoUniqueKey{
            .mFilePath = filename.str(),
            .mContextPath = path,
            .mVariableName = variableName,
        };

        auto it = MetaInfoManager->mAllClassMetaInfo.find( uniqueKey );
        if ( it != MetaInfoManager->mAllClassMetaInfo.end() )
        {
            return;
        }

        ClassMetaInfo classMetaInfo;
        classMetaInfo.mMetaInfo.mId = MetaIdGenerator::Next();
        classMetaInfo.mClassName = RD->getNameAsString();
        classMetaInfo.mFilePath = uniqueKey.mFilePath;
        classMetaInfo.mIsFinal = RD->isEffectivelyFinal();

        clang::ASTContext* Context = Result.Context;
        const clang::RawComment* rawComment = Context->getRawCommentForDeclNoCache( RD );
        if ( rawComment )
        {
            classMetaInfo.mMetaInfo.mComment = rawComment->getRawText( Context->getSourceManager() );
        }

        for ( const clang::Decl* decl : RD->decls() )
        {
            if ( const auto* field = llvm::dyn_cast<clang::FieldDecl>( decl ) )
            {
                bool bFieldHasAttr = false;
                for ( const clang::Attr* fieldAttr : field->attrs() )
                {
                    auto* fieldAnnotate = llvm::dyn_cast<clang::AnnotateAttr>( fieldAttr );
                    if ( fieldAnnotate )
                    {
                        auto annotation = fieldAnnotate->getAnnotation();
                        if ( annotation.starts_with( "NAYUKI::" ) )
                        {
                            bFieldHasAttr = true;
                        }
                    }
                }

                if ( bFieldHasAttr == false )
                {
                    continue;
                }

                PropertyMetaInfo propertyMetaInfo;
                propertyMetaInfo.mMetaInfo.mId = MetaIdGenerator::Next();
                propertyMetaInfo.mAccessLevel = getAccessSpelling( field->getAccess() );

                clang::QualType fieldType = field->getType();
                propertyMetaInfo.mPropertyType = fieldType.getAsString();
                propertyMetaInfo.mPropertyName = field->getNameAsString();

                // 是引用或者指针类型
                propertyMetaInfo.mIsPtr = fieldType->isPointerType();
                propertyMetaInfo.mIsLVRef = fieldType->isLValueReferenceType();
                propertyMetaInfo.mIsRVRef = fieldType->isRValueReferenceType();
                // TODO: 没有处理ptr的const等限定符
                clang::QualType targetType =
                    ( propertyMetaInfo.mIsLVRef || propertyMetaInfo.mIsRVRef || propertyMetaInfo.mIsPtr )
                        ? fieldType->getPointeeType()
                        : fieldType;

                clang::Qualifiers qualifiers = targetType.getQualifiers();
                propertyMetaInfo.mIsConst = qualifiers.hasConst();
                propertyMetaInfo.mIsVolatile = qualifiers.hasVolatile();

                classMetaInfo.mProperties.push_back( propertyMetaInfo );
            }
            else if ( const auto* var = llvm::dyn_cast<clang::VarDecl>( decl ) )
            {
                bool bVarHasAttr = false;
                for ( const clang::Attr* varAttr : var->attrs() )
                {
                    auto* varAnnotate = llvm::dyn_cast<clang::AnnotateAttr>( varAttr );
                    if ( varAnnotate )
                    {
                        auto annotation = varAnnotate->getAnnotation();
                        if ( annotation.starts_with( "NAYUKI::" ) )
                        {
                            bVarHasAttr = true;
                        }
                    }
                }

                if ( bVarHasAttr == false )
                {
                    continue;
                }

                ValueMetaInfo valueMetaInfo;
                valueMetaInfo.mMetaInfo.mId = MetaIdGenerator::Next();
                valueMetaInfo.mAccessLevel = getAccessSpelling( var->getAccess() );

                clang::QualType varType = var->getType();
                valueMetaInfo.mValueType = varType.getAsString();
                valueMetaInfo.mValueName = var->getNameAsString();

                valueMetaInfo.mIsInline = var->isInline();
                valueMetaInfo.mIsStatic = var->isStaticDataMember();
                valueMetaInfo.mIsConstexpr = var->isConstexpr();

                // 是引用或者指针类型
                valueMetaInfo.mIsPtr = varType->isPointerType();
                valueMetaInfo.mIsLVRef = varType->isLValueReferenceType();
                valueMetaInfo.mIsRVRef = varType->isRValueReferenceType();
                // TODO: 没有处理ptr的const等限定符
                clang::QualType targetType =
                    ( valueMetaInfo.mIsLVRef || valueMetaInfo.mIsRVRef || valueMetaInfo.mIsPtr )
                        ? varType->getPointeeType()
                        : varType;

                clang::Qualifiers qualifiers = targetType.getQualifiers();
                valueMetaInfo.mIsConst = qualifiers.hasConst();
                valueMetaInfo.mIsVolatile = qualifiers.hasVolatile();

                classMetaInfo.mValues.push_back( valueMetaInfo );
            }
            else if ( const auto* function = llvm::dyn_cast<clang::FunctionDecl>( decl ) )
            {
                bool bFunctionHasAttr = false;
                for ( const clang::Attr* functionAttr : function->attrs() )
                {
                    auto* functionAnnotate = llvm::dyn_cast<clang::AnnotateAttr>( functionAttr );
                    if ( functionAnnotate )
                    {
                        auto annotation = functionAnnotate->getAnnotation();
                        if ( annotation.starts_with( "NAYUKI::" ) )
                        {
                            bFunctionHasAttr = true;
                        }
                    }
                }

                if ( bFunctionHasAttr == false )
                {
                    continue;
                }

                if ( const auto* method = llvm::dyn_cast<clang::CXXMethodDecl>( decl ) )
                {
                    if ( method->isStatic() )
                    {
                        FunctionMetaInfo functionMetaInfo;
                        functionMetaInfo.mMetaInfo.mId = MetaIdGenerator::Next();

                        const clang::RawComment* methodRawComment = Context->getRawCommentForDeclNoCache( method );
                        if ( methodRawComment )
                        {
                            functionMetaInfo.mMetaInfo.mComment =
                                methodRawComment->getRawText( Context->getSourceManager() );
                        }

                        functionMetaInfo.mAccessLevel = getAccessSpelling( method->getAccess() );
                        functionMetaInfo.mIsConsteval = method->isConstexpr();
                        functionMetaInfo.mIsConsteval = method->isConsteval();
                        functionMetaInfo.mFunctionName = method->getNameAsString();
                        functionMetaInfo.mFunctionType = method->getType().getAsString();

                        for ( uint32_t i = 0; i < method->getNumParams(); ++i )
                        {
                            const auto* parmDecl = method->getParamDecl( i );
                            FunctionParam param;
                            param.mName = parmDecl->getNameAsString();
                            functionMetaInfo.mFunctionParams.push_back( param );
                        }

                        classMetaInfo.mFunctions.push_back( functionMetaInfo );
                    }
                    else
                    {

                        MethodMetaInfo methodMetaInfo;
                        methodMetaInfo.mMetaInfo.mId = MetaIdGenerator::Next();

                        const clang::RawComment* methodRawComment = Context->getRawCommentForDeclNoCache( method );
                        if ( methodRawComment )
                        {
                            methodMetaInfo.mMetaInfo.mComment =
                                methodRawComment->getRawText( Context->getSourceManager() );
                        }

                        methodMetaInfo.mAccessLevel = getAccessSpelling( method->getAccess() );
                        methodMetaInfo.mIsConst = method->isConst();
                        methodMetaInfo.mIsConsteval = method->isConstexpr();
                        methodMetaInfo.mIsConsteval = method->isConsteval();
                        methodMetaInfo.mMethodName = method->getNameAsString();
                        methodMetaInfo.mMethodType = method->getType().getAsString();

                        for ( uint32_t i = 0; i < method->getNumParams(); ++i )
                        {
                            const auto* parmDecl = method->getParamDecl( i );
                            MethodParam param;
                            param.mName = parmDecl->getNameAsString();
                            methodMetaInfo.mMethodParams.push_back( param );
                        }

                        classMetaInfo.mMethods.push_back( methodMetaInfo );
                    }
                }
                else
                {
                }
            }
        }

        MetaInfoManager->mAllClassMetaInfo.insert( { uniqueKey, classMetaInfo } );
    }

  protected:
    std::shared_ptr<MetaInfoManager> MetaInfoManager;
};
