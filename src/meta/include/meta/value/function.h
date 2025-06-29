#pragma once

#include "meta/user_type/define.h"
#include "meta/user_type/namespace.h"
#include "meta/value/fn_ptr_trait.h"
#include "meta/value/value.h"

class META_API Function : public Value
{
  public:
    Function()
    {
    }

    template <auto FuncPtr>
    void Create( std::string& name, std::unique_ptr<Namespace> ns = nullptr )
    {
        // 看看能不能靠funcTrait将参数类型提取出来然后判断，将结果存入buffer？？？？？？？？
        mName = name;
        mFunc = FnPtrTrait<FuncPtr>();
        mNamespace = std::move( ns );
    }

    void SetAccessLevel( AccessLevel accessLevel )
    {
        mAccessLevel = accessLevel;
    }

    template <typename... TArgs>
    void invoke( TArgs... args )
    {
        if ( mFunc )
        {
            mFunc->Invoke( FnParamValues{ nullptr, { std::any( args )... } } );
        }
    }

  protected:
    // 访问级别
    AccessLevel mAccessLevel = AccessLevel::Public;

    std::string mName;

    FnPtrInfo* mFunc = nullptr; // 函数指针信息

    std::unique_ptr<Namespace> mNamespace = nullptr;
};