
#include <core/core.h>
#include <core/memory/global_new.h>

#include <meta_forge/meta_forge.h>

NY_LOG_CATEGORY_DEFINITION( LogMetaForge )

#define STRUCT(name) [[clang::annotate(#name)]]
#define PROPERTY(...) [[clang::annotate(#__VA_ARGS__)]]

#define usize size_t

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