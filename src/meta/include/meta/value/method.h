#pragma once

#include "core/traits/fn_trait.h"
#include "meta/user_type/define.h"
#include "meta/value/value.h"
#include <core/minimal.h>
#include <meta/minimal.h>

struct META_API Params
{
    std::vector<TypeId> mTypeIds;
};

// 使用模板。无法通过基类传递参数和返回值。
class META_API Method : public Value
{
  public:
    Method();

    template <typename TFunc>
    void Create( TFunc func, std::string& name )
    {
        // 看看能不能靠funcTrait将参数类型提取出来然后判断，将结果存入buffer？？？？？？？？
        mFunc = func;
        mName = name;

        std::apply(
            [this]( auto... args )
            {
                // 这里可以将参数类型存入mParams中
                // 例如：mParams.mTypeIds.push_back(TypeIdGenerator::Get<TypeIdOfArgs>());
                ( mParams.mTypeIds.push_back( TypeIdGenerator::Get<std::decay_t<decltype( args )>>() ), ... );
            },
            FnTrait<TFunc>::ParamsTypeTuple );
    }

    void SetAccessLevel( AccessLevel accessLevel )
    {
        mAccessLevel = accessLevel;
    }

    template <typename... TArgs>
    void invoke( void* object, TArgs... args )
    {
        UNUSED_VARS( object );
        UNUSED_VARS_PACK( args );

        // mParams

        // 调用成员函数
        if ( mFunc )
        {
        }
    }

  protected:
    // 访问级别
    AccessLevel mAccessLevel = AccessLevel::Public;

    std::string mName;
    // 函数指针
    void* mFunc = nullptr;

    Params mParams;
};