#pragma once

#include <module_export.h>

class CORE_API NonCopyable
{
  protected:
    NonCopyable() = default;

    virtual ~NonCopyable()
    {
    }

  private:
    NonCopyable& operator=( const NonCopyable& ) = delete;
    NonCopyable( const NonCopyable& ) = delete;
};
