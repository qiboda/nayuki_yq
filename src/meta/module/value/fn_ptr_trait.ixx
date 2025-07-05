module;

#include <module_export.h>
#include <core/macro/macro.h>
// #include <core/logger/log.h>
// #include <fmt/format.h>

export module meta.value.fn_ptr_trait;

import std;
import core;
import meta;
import core.logger.logger;

export enum class FnPtrKind : u8 {
    None = 0,
    Normal = 1, // 普通函数指针
    Member = 2, // 成员函数指针
};

export struct META_API FnParamValues
{
    void* mObject = nullptr; // 成员函数指针的对象指针

    // 存储参数的指针
    std::vector<std::any> mParams;
};

export class META_API FnPtrInfo
{
  public:
    FnPtrInfo() = default;

    virtual ~FnPtrInfo() = default;

    virtual std::any Invoke( FnParamValues&& params ) const = 0;
};

export template <auto FnPtr>
struct FnPtrTrait : std::false_type, public FnPtrInfo
{
    static_assert( false );

    virtual std::any Invoke( FnParamValues&& params ) const override
    {
        UNUSED_VARS(params);
        return {};
    }
};

export template <typename TRet, typename... TArgs, TRet ( *FnPtr )( TArgs... )>
struct FnPtrTrait<FnPtr> : std::true_type, public FnPtrInfo
{
    using Type = TRet ( * )( TArgs... );

    static inline constexpr FnPtrKind Kind = FnPtrKind::Normal;

    template <usize... I>
    TRet InvokeImpl( const std::vector<std::any>& params, std::index_sequence<I...> ) const
    {
        return std::invoke( FnPtr, std::any_cast<std::decay_t<TArgs>>( params[I] )... );
    }

    virtual std::any Invoke( FnParamValues&& params ) const override
    {
        if ( params.mParams.size() != sizeof...( TArgs ) )
        {
            // 此处使用log，会导致obj文件膨胀超过4G，无法编译。
            // NY_LOG_ERROR( LogMeta, "function params count error {} != {}", params.mParams.size(), sizeof...( TArgs )
            // );
            return {};
        }

        if constexpr ( std::is_void_v<TRet> )
        {
            this->InvokeImpl( params.mParams, std::index_sequence_for<TArgs...>{} );
            return {};
        }
        else
        {
            return this->InvokeImpl( params.mParams, std::index_sequence_for<TArgs...>{} );
        }
    }
};

export template <typename TClass, typename TRet, typename... TArgs, TRet ( TClass::*FnPtr )( TArgs... )>
struct FnPtrTrait<FnPtr> : std::true_type, public FnPtrInfo
{
    using Type = TRet ( TClass::* )( TArgs... );

    static inline constexpr FnPtrKind Kind = FnPtrKind::Member;

    virtual std::any Invoke( FnParamValues&& params ) const override
    {
        if ( params.mParams.size() != sizeof...( TArgs ) )
        {
            // 此处使用log，会导致obj文件膨胀超过4G，无法编译。
            // NY_LOG_ERROR( LogMeta,
            //               "function params count error {} != {}",
            //               //   FmtHelper::MemberFnPtr( FnPtr ),
            //               params.mParams.size(),
            //               sizeof...( TArgs ) );
            return {};
        }

        if constexpr ( std::is_void_v<TRet> )
        {
            this->InvokeImpl( static_cast<TClass*>( params.mObject ),
                              params.mParams,
                              std::index_sequence_for<TArgs...>{} );
            return {};
        }
        else
        {
            return this->InvokeImpl( static_cast<TClass*>( params.mObject ),
                                     params.mParams,
                                     std::index_sequence_for<TArgs...>{} );
            return {};
        }
    }

  private:
    template <usize... I>
    TRet InvokeImpl( TClass* object, const std::vector<std::any>& params, std::index_sequence<I...> ) const
    {
        return std::invoke( FnPtr, object, std::any_cast<std::decay_t<TArgs>>( params[I] )... );
    }
};
