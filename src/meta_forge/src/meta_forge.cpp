
#include <meta_forge/meta_forge.h>
#include <cstddef>
#include <utility>

#define STRUCT(name) [[clang::annotate(#name)]]
#define PROPERTY(...) [[clang::annotate(#__VA_ARGS__)]]

#define usize size_t

/**
 * 注释内容说明
 */
struct [[clang::annotate("my_struct")]] MyStruct
{
   PROPERTY(field, name)
    int a;
};

struct [[my_attr]] MyStructA
{
    int a;
};

struct STRUCT(Reflect) MyStructAs
{
    usize a;
};