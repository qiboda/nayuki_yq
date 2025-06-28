#pragma once

#include "meta/meta.h"
#include <core/minimal.h>
#include <meta/minimal.h>

enum class FnPtrKind : u8
{
    None = 0,
    Normal = 1, // 普通函数指针
    Member = 2, // 成员函数指针
};

struct META_API FnParamValues
{
    void* mObject = nullptr; // 成员函数指针的对象指针

    // 存储参数的指针
    std::vector<std::any> mParams;
};

class META_API FnPtrInfo
{
  public:
    FnPtrInfo() = default;

    virtual ~FnPtrInfo() = default;

    virtual std::any Invoke( FnParamValues&& params ) const = 0;
};

template <auto FnPtr>
struct FnPtrTrait : std::false_type
{
    static_assert( false );
};

template <typename TRet, typename... TArgs, TRet ( *FnPtr )( TArgs... )>
struct FnPtrTrait<FnPtr> : std::true_type, public FnPtrInfo
{
    using Type = TRet ( * )( TArgs... );

    static inline constexpr FnPtrKind Kind = FnPtrKind::Normal;

    template <usize... I>
    TRet InvokeImpl( const std::vector<std::any>& params, std::index_sequence<I...> ) const
    {
        return std::invoke( FnPtr, std::any_cast<std::decay_t<TArgs>>( params[I] )... );
    }

    virtual std::any Invoke( FnParamValues&& params ) const
    {
        if ( params.mParams.size() != sizeof...( TArgs ) )
        {
            NY_LOG_ERROR( LogMeta,
                          "function {} params count error {} != {}",
                          FmtHelper::MemberFnPtr( FnPtr ),
                          params.mParams.size(),
                          sizeof...( TArgs ) );
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

template <typename TClass, typename TRet, typename... TArgs, TRet ( TClass::*FnPtr )( TArgs... )>
struct FnPtrTrait<FnPtr> : std::true_type, public FnPtrInfo
{
    using Type = TRet ( TClass::* )( TArgs... );

    static inline constexpr FnPtrKind Kind = FnPtrKind::Member;

    template <usize... I>
    TRet InvokeImpl( TClass* object, const std::vector<std::any>& params, std::index_sequence<I...> ) const
    {
        return std::invoke( FnPtr, object, std::any_cast<std::decay_t<TArgs>>( params[I] )... );
    }

    virtual std::any Invoke( FnParamValues&& params ) const
    {
        if ( params.mParams.size() != sizeof...( TArgs ) )
        {
            NY_LOG_ERROR( LogMeta,
                          "function {} params count error {} != {}",
                          FmtHelper::MemberFnPtr( FnPtr ),
                          params.mParams.size(),
                          sizeof...( TArgs ) );
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
        }
    }
};
