
#include <meta_core/meta_macro.h>
#include <vector>

/**
 * 注释内容说明
 */
class CLASS( Hello ) MyClass
{
  public:
    MyClass()
    {
    }

    void Test()
    {
    }

  private:
    METHOD()
    int getMe( void* name, float yes ) const
    {
        return 0;
    }

    METHOD()
    constexpr int getMe( void* name, float yes )
    {
        return 0;
    }

    METHOD()
    consteval int getconsteval( void* name, float yes ) const
    {
        return 0;
    }

    METHOD()
    consteval int getMeMove( void* name, float yes ) &&
    {
        return 0;
    }

    METHOD()
    int getMeRef( void* name, float yes ) const&
    {
        return 0;
    }

    FUNCTION()
    static int getFunction( void* name, float yes )
    {
        return 0;
    }

    METHOD()
    inline int getMeInline( void* name, float yes ) const&
    {
        return 0;
    }
};
