
#include <meta_forge/meta_forge.h>
#include <cstddef>
#include <utility>

#include "meta_forge/meta_macro.h"

#define usize size_t

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

struct [[my_attr]] MyStructA
{
    int a;
};

struct STRUCT( Reflect ) MyStructAs
{
    usize a;
};