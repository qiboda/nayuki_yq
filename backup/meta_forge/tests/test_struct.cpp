
#define usize size_t

#include <meta_core/meta_macro.h>
#include <vector>

/**
 * 注释内容说明
 */
struct STRUCT( Hello ) MyStruct
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
};

struct STRUCT( Reflect ) MyStructAs
{
    usize a;
};