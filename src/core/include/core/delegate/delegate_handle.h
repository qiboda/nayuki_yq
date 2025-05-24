#pragma once

#include <module_export.h>
#include <core/type_def.h>

class CORE_API DelegateHandle
{
  public:
    enum class GenerateNewHandleType
    {
        GenerateNewHandle
    };

  public:
    DelegateHandle()
        : mHandleId( 0u )
    {
    }

    DelegateHandle( const DelegateHandle & ) = default;
    DelegateHandle( DelegateHandle && ) = default;
    DelegateHandle &operator=( const DelegateHandle & ) = default;
    DelegateHandle &operator=( DelegateHandle && ) = default;

    explicit DelegateHandle( GenerateNewHandleType )
        : mHandleId( GenerateNewId() )
    {
    }

    ~DelegateHandle()
    {
    }

  public:
    u64 GenerateNewId();

  public:
    operator bool() const
    {
        return IsValid();
    }

  public:
    bool IsValid() const
    {
        return mHandleId != 0u;
    }

    void Invalid()
    {
        mHandleId = 0u;
    }

  private:
    friend bool operator==( const DelegateHandle &Lhs, const DelegateHandle &Rhs )
    {
        return Lhs.mHandleId == Rhs.mHandleId;
    }

  private:
    u64 mHandleId = 0u;
};