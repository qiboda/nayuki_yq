#pragma once

#include "core/minimal.h"

class CORE_API NonCopyable
{
  protected:
    NonCopyable() = default;

    virtual ~NonCopyable()
    {
    }

  private:
    NonCopyable &operator=( const NonCopyable & ) = delete;
    NonCopyable( const NonCopyable & ) = delete;
};
