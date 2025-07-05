module;

#include <module_export.h>

export module meta.value.method;

import core;

import meta.user_type.define;
import meta.value.fn_ptr_trait;
import meta.value.value;

// 使用模板。无法通过基类传递参数和返回值。
export class META_API Method : public Value
{
  public:
    Method();

    virtual ~Method() override
    {
        if ( mFunc )
        {
            delete mFunc;
        }
    }

    template <auto FuncPtr>
    void Create( const std::string& name, AccessLevel accessLevel )
    {
        // 看看能不能靠funcTrait将参数类型提取出来然后判断，将结果存入buffer？？？？？？？？
        mName = name;
        mFunc = new FnPtrTrait<FuncPtr>();
        mAccessLevel = accessLevel;
    }

    template <typename TClass, typename... TArgs>
    void invoke( TClass* object, TArgs... args )
    {
        if ( mFunc )
        {
            mFunc->Invoke( FnParamValues{ object, { std::any( args )... } } );
        }
    }

  protected:
    // 访问级别
    AccessLevel mAccessLevel = AccessLevel::Public;

    std::string mName;

    FnPtrInfo* mFunc = nullptr; // 函数指针信息
};