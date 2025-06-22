
#include <meta_forge/meta_macro.h>
#include <vector>

/**
 * 注释内容说明
 */
class CLASS( Hello ) MyClass
{
    PROPERTY( field, name )
    int a;

    PROPERTY( field, name )
    int& aa;

    PROPERTY( field, name )
    const int& aaa;

    PROPERTY( field, name )
    const int&& s;

    PROPERTY( field, name )
    volatile const int&& ssss;

    PROPERTY( field, name )
    volatile const int* sssss;

    PROPERTY( field, name )
    const std::vector<int>&& k;

    VALUE( static )
    constexpr const static inline volatile int* j = nullptr;
};

namespace A
{
namespace B
{
class CLASS( Reflect ) MyClass
{

    float a;

    class CLASS( Reflect ) MyClassInner
    {
    };
};
} // namespace B
} // namespace A

template <typename T>
class CLASS( Template ) C
{
};

namespace A
{

namespace C
{
template <typename T>
class CLASS( Template ) C
{
};
} // namespace C
} // namespace A

namespace A
{
namespace C
{
template <typename T>
class CLASS( Template ) D
{
    template <typename TD, typename TDD>
    class CLASS( Template ) DDD
    {
    };
};
} // namespace C
} // namespace A
