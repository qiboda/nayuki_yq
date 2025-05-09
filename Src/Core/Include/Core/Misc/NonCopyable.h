#pragma once

#include <Core/Core.h>

class CORE_API NonCopyable
{
  protected:
    NonCopyable() {}
    ~NonCopyable() {}

  private:
    NonCopyable &operator=( const NonCopyable & ) = delete;
    NonCopyable( const NonCopyable & ) = delete;
};
