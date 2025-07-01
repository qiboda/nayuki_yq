module;

#include <module_export.h>

export module core.misc:non_copyable;

export class CORE_API NonCopyable
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
