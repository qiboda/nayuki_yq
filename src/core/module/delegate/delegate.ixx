module;

#include <core/macro/macro.h>

export module core.delegate:delegate;

import :handle;
import :instance;

import fmt;

// To bind function: only need to same parameters and return value;
// TReturnVal should be always void type.
export template <typename TReturnVal, typename... TArgs>
class MultipleDelegate
{
  public:
    using DelegateInstanceType = DelegateInstance<TReturnVal, TArgs...>*;

  public:
#pragma region ThreeFive

    constexpr MultipleDelegate()
    {
    }

    ~MultipleDelegate()
    {
        for ( DelegateInstanceType delegateInstance : mDelegateInstances )
        {
            NY_DELETE( delegateInstance );
        }
        mDelegateInstances.clear();
    }

    MultipleDelegate( const MultipleDelegate& multipleDelegate )
    {
        for ( auto delegateInstance : multipleDelegate.mDelegateInstances )
        {
            if ( delegateInstance )
            {
                mDelegateInstances.push_back( delegateInstance->Clone() );
            }
            else
            {
                mDelegateInstances.push_back( nullptr );
            }
        }
    }

    MultipleDelegate( MultipleDelegate&& multipleDelegate ) noexcept
        : mDelegateInstances( std::move( multipleDelegate.mDelegateInstances ) )
    {
        multipleDelegate.mDelegateInstances.clear();
    }

    MultipleDelegate& operator=( const MultipleDelegate& multipleDelegate )
    {
        for ( auto delegateInstance : mDelegateInstances )
        {
            if ( delegateInstance )
            {
                NY_DELETE( delegateInstance );
            }
        }
        mDelegateInstances.clear();

        for ( auto delegateInstance : multipleDelegate.mDelegateInstances )
        {
            if ( delegateInstance )
            {
                mDelegateInstances.push_back( delegateInstance->Clone() );
            }
            else
            {
                mDelegateInstances.push_back( nullptr );
            }
        }
        return *this;
    }

    MultipleDelegate& operator=( MultipleDelegate&& multipleDelegate ) noexcept
    {
        mDelegateInstances = std::move( multipleDelegate.mDelegateInstances );
        multipleDelegate.mDelegateInstances.clear();
        return *this;
    }

#pragma endregion ThreeFive

  public:
#pragma region Add

    // function, static function, static member function and include lambda
    DelegateHandle Add( typename DelegateFunInstance<TReturnVal, TArgs...>::TFunctor functor )
    {
        DelegateHandle delegateHandle( DelegateHandle::GenerateNewHandleType::GenerateNewHandle );
        DelegateInstanceType Instance = new DelegateFunInstance<TReturnVal, TArgs...>( functor );
        Instance->SetHandle( delegateHandle );
        mDelegateInstances.push_back( Instance );
        return delegateHandle;
    }

    // member function, intrusive
    template <typename TClass>
    DelegateHandle AddMem( typename DelegateMemFunInstance<TClass, TReturnVal, TArgs...>::TFunctor functor,
                           TClass* object )
    {
        DelegateHandle delegateHandle( DelegateHandle::GenerateNewHandleType::GenerateNewHandle );
        DelegateInstanceType Instance = new DelegateMemFunInstance<TClass, TReturnVal, TArgs...>( functor, object );
        Instance->SetHandle( delegateHandle );
        mDelegateInstances.push_back( Instance );
        return delegateHandle;
    }

    // member function, shared member
    template <typename TClass>
    DelegateHandle AddSP( typename DelegateSPMemFunInstance<TClass, TReturnVal, TArgs...>::TFunctor functor,
                          std::shared_ptr<TClass> SPObject )
    {
        DelegateHandle delegateHandle( DelegateHandle::GenerateNewHandleType::GenerateNewHandle );
        DelegateInstanceType Instance = new DelegateSPMemFunInstance<TClass, TReturnVal, TArgs...>( functor, SPObject );
        Instance->SetHandle( delegateHandle );
        mDelegateInstances.push_back( Instance );
        return delegateHandle;
    }

#pragma endregion Add

  public:
#pragma region Remove

    bool Remove( DelegateHandle delegateHandle )
    {
        auto remove_it = std::find_if( mDelegateInstances.begin(),
                                       mDelegateInstances.end(),
                                       [&delegateHandle]( const DelegateInstanceType delegateInstance )
                                       {
                                           if ( delegateInstance->GetHandle() == delegateHandle )
                                           {
                                               return true;
                                           }
                                           return false;
                                       } );

        NY_DELETE( *remove_it );

        bool is_remove = ( remove_it != mDelegateInstances.end() );
        mDelegateInstances.erase( remove_it );
        return is_remove;
    }

    bool RemoveAll()
    {
        bool is_empty = mDelegateInstances.empty();
        for ( DelegateInstanceType delegateInstance : mDelegateInstances )
        {
            NY_DELETE( delegateInstance );
        }
        mDelegateInstances.clear();
        return !is_empty;
    }

#pragma endregion Remove

  public:
#pragma region broadcast

    void Broadcast( TArgs... args )
    {
        for ( auto&& DelegateInstance : mDelegateInstances )
        {
            ( *DelegateInstance )( args... );
        }
    }

#pragma endregion broadcast

  private:
    std::vector<DelegateInstanceType> mDelegateInstances;
};

// To bind function: only need to same parameters and return value;
export template <typename TReturnVal, typename... TArgs>
class SingleDelegate
{
  public:
    using DelegateInstanceType = DelegateInstance<TReturnVal, TArgs...>*;

  public:
#pragma region ThreeFive

    constexpr SingleDelegate()
        : mDelegateInstance( nullptr )
    {
    }

    ~SingleDelegate()
    {
        NY_DELETE( mDelegateInstance );
    }

    SingleDelegate( const SingleDelegate& singleDelegate ) noexcept
        : mDelegateInstance( singleDelegate.mDelegateInstance == nullptr ? nullptr
                                                                         : singleDelegate.mDelegateInstance->Clone() )
    {
    }

    SingleDelegate( SingleDelegate&& singleDelegate ) noexcept
        : mDelegateInstance( std::move( singleDelegate.mDelegateInstance ) )
    {
        singleDelegate.mDelegateInstance = nullptr;
    }

    SingleDelegate& operator=( const SingleDelegate& singleDelegate )
    {
        if ( singleDelegate.mDelegateInstance )
        {
            mDelegateInstance = singleDelegate.mDelegateInstance->Clone();
        }
        else
        {
            mDelegateInstance = nullptr;
        }
        return *this;
    }

    SingleDelegate& operator=( SingleDelegate&& singleDelegate ) noexcept
    {
        mDelegateInstance = std::move( singleDelegate.mDelegateInstance );
        singleDelegate.mDelegateInstance = nullptr;
        return *this;
    }

#pragma endregion ThreeFive

  public:
#pragma region Bind

    // function, static function, static member function and include lambda
    DelegateHandle Bind( typename DelegateFunInstance<TReturnVal, TArgs...>::TFunctor functor )
    {
        NY_ASSERT_MSG( mDelegateInstance == nullptr, "SingleDelegate only binds once" )

        DelegateHandle delegateHandle( DelegateHandle::GenerateNewHandleType::GenerateNewHandle );
        mDelegateInstance = new DelegateFunInstance<TReturnVal, TArgs...>( functor );
        mDelegateInstance->SetHandle( delegateHandle );
        return mDelegateInstance->GetHandle();
    }

    // member function, intrusive
    template <typename TClass>
    DelegateHandle BindMem( typename DelegateMemFunInstance<TClass, TReturnVal, TArgs...>::TFunctor functor,
                            TClass* object )
    {
        NY_ASSERT_MSG_MSG( mDelegateInstance == nullptr, L"SingleDelegate only binds once" );

        DelegateHandle delegateHandle( DelegateHandle::GenerateNewHandleType::GenerateNewHandle );
        mDelegateInstance = new DelegateMemFunInstance<TClass, TReturnVal, TArgs...>( functor, object );
        mDelegateInstance->SetHandle( delegateHandle );
        return mDelegateInstance->GetHandle();
    }

    // member function, shared member
    template <typename TClass>
    DelegateHandle BindSP( typename DelegateSPMemFunInstance<TClass, TReturnVal, TArgs...>::TFunctor functor,
                           std::shared_ptr<TClass> SPObject )
    {
        NY_ASSERT_MSG_MSG( mDelegateInstance == nullptr, L"SingleDelegate only binds once" );

        DelegateHandle delegateHandle( DelegateHandle::GenerateNewHandleType::GenerateNewHandle );
        mDelegateInstance = new DelegateSPMemFunInstance<TClass, TReturnVal, TArgs...>( functor, SPObject );
        mDelegateInstance->SetHandle( delegateHandle );
        return mDelegateInstance->GetHandle();
    }

#pragma endregion Bind
  public:
#pragma region Unbind

    bool Unbind()
    {
        if ( mDelegateInstance )
        {
            mDelegateInstance = nullptr;
            return true;
        }
        return false;
    }

    bool Unbind( DelegateHandle delegateHandle )
    {
        if ( mDelegateInstance && mDelegateInstance->GetHandle() == delegateHandle )
        {
            return Unbind();
        }
        return false;
    }

#pragma endregion Unbind

  public:
#pragma region Execute

    TReturnVal Execute( TArgs... args )
    {
        NY_ASSERT_MSG( mDelegateInstance != nullptr, "SingleDelegate must bind a function" )
        return ( *mDelegateInstance )( args... );
    }

#pragma endregion Execute

  private:
    DelegateInstanceType mDelegateInstance;
};
