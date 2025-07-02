module;

#include <core/macro/macro.h>

export module core.delegate:instance;

import std;
import :handle;
// import core.delegate:delegate;

export template <typename TReturnVal, typename... TArgs>
class DelegateInstance
{
  public:
    using Type = TReturnVal ( * )( TArgs... );

  public:
    DelegateInstance()
    {
    }

    explicit DelegateInstance( const DelegateHandle& delegateHandle )
        : mDelegateHandle( delegateHandle )
    {
    }

    virtual ~DelegateInstance()
    {
    }

    virtual TReturnVal operator()( TArgs... args ) const = 0;
    virtual DelegateInstance<TReturnVal, TArgs...>* Clone() const = 0;

  public:
    const DelegateHandle& GetHandle() const
    {
        return mDelegateHandle;
    }

    DelegateHandle& GetHandle()
    {
        return mDelegateHandle;
    }

    void SetHandle( DelegateHandle delegateHandle )
    {
        mDelegateHandle = delegateHandle;
    }

  private:
    DelegateHandle mDelegateHandle;
};

export template <typename TReturnVal, typename... TArgs>
class DelegateFunInstance : public DelegateInstance<TReturnVal, TArgs...>
{
  public:
    using TFunctor = std::function<TReturnVal( TArgs... )>;

  public:
    DelegateFunInstance( const TFunctor& functor )
        : DelegateInstance<TReturnVal, TArgs...>()
        , mFunctor( functor )
    {
    }

    DelegateFunInstance( const TFunctor& functor, const DelegateHandle& delegateHandle )
        : DelegateInstance<TReturnVal, TArgs...>( delegateHandle )
        , mFunctor( functor )
    {
    }

    virtual ~DelegateFunInstance() override
    {
    }

  public:
    virtual TReturnVal operator()( TArgs... args ) const override
    {
        return mFunctor( args... );
    }

    virtual DelegateInstance<TReturnVal, TArgs...>* Clone() const override
    {
        return new DelegateFunInstance( mFunctor, this->GetHandle() );
    }

  private:
    TFunctor mFunctor;
};

export template <typename TClass, typename TReturnVal, typename... TArgs>
class DelegateMemFunInstance : public DelegateInstance<TReturnVal, TArgs...>
{
  public:
    using TFunctor = TReturnVal ( TClass::* )( TArgs... );

  public:
    DelegateMemFunInstance( TFunctor functor, TClass* object )
        : DelegateInstance<TReturnVal, TArgs...>()
        , mFunctor( functor )
        , mObject( object )
    {
    }

    DelegateMemFunInstance( TFunctor functor, TClass* object, const DelegateHandle& delegateHandle )
        : DelegateInstance<TReturnVal, TArgs...>( delegateHandle )
        , mFunctor( functor )
        , mObject( object )
    {
    }

    virtual ~DelegateMemFunInstance() override
    {
        mFunctor = nullptr;
        mObject = nullptr;
    }

  public:
    virtual TReturnVal operator()( TArgs... args ) const override
    {
        if ( mObject )
        {
            return ( mObject->*mFunctor )( args... );
        }
        return TReturnVal();
    }

    virtual DelegateInstance<TReturnVal, TArgs...>* Clone() const override
    {
        return new DelegateMemFunInstance( mFunctor, mObject, this->GetHandle() );
    }

  private:
    TFunctor mFunctor;
    TClass* mObject;
};

export template <typename TClass, typename TReturnVal, typename... TArgs>
class DelegateSPMemFunInstance : public DelegateInstance<TReturnVal, TArgs...>
{
  public:
    using TFunctor = TReturnVal ( TClass::* )( TArgs... );

  public:
    DelegateSPMemFunInstance( TFunctor functor, std::shared_ptr<TClass> SPObject )
        : DelegateInstance<TReturnVal, TArgs...>()
        , mFunctor( functor )
        , mWPObject( SPObject )
    {
    }

    DelegateSPMemFunInstance( TFunctor functor, std::shared_ptr<TClass> SPObject, const DelegateHandle& delegateHandle )
        : DelegateInstance<TReturnVal, TArgs...>( delegateHandle )
        , mFunctor( functor )
        , mWPObject( SPObject )
    {
    }

    virtual ~DelegateSPMemFunInstance() override
    {
        mFunctor = nullptr;
        mWPObject.reset();
    }

  public:
    virtual TReturnVal operator()( TArgs... args ) const override
    {
        if ( mWPObject.expired() )
        {
            return TReturnVal();
        }
        auto SPObject = mWPObject.lock();
        return ( ( *SPObject ).*mFunctor )( args... );
    }

    virtual DelegateInstance<TReturnVal, TArgs...>* Clone() const override
    {
        return new DelegateSPMemFunInstance( mFunctor, mWPObject.lock(), this->GetHandle() );
    }

  private:
    TFunctor mFunctor;
    std::weak_ptr<TClass> mWPObject;
};
