#pragma once

#ifndef MACRO_H
#define MACRO_H

#include <NayukiYq/TemplateUtility.h>

#include <cassert>
#include <cwchar>

#pragma region disable_warning

// to remove unused variable warning
#define UNUSED_VAR(var) ((void)&var)

#pragma endregion disable_warning

#ifndef NY_PRE_CONDITION
#define NY_PRE_CONDITION(expr, msg, ...)                                       \
    {                                                                          \
        if ((expr) == false) {                                                 \
            std::cerr << msg << ##__VA_ARGS__ << std::endl;                    \
            assert(expr);                                                      \
        }                                                                      \
    }
#endif // !NY_PRE_CONDITION

#ifndef NY_POST_CONDITION
#define NY_POST_CONDITION(expr, msg, ...)                                      \
    {                                                                          \
        if ((expr) == false) {                                                 \
            std::cerr << msg << ##__VA_ARGS__ << std::endl;                    \
            assert(expr);                                                      \
        }                                                                      \
    }
#endif // !NY_POST_CONDITION

#ifndef NY_ASSERT
#define NY_ASSERT(expr, msg, ...)                                              \
    {                                                                          \
        if ((expr) == false) {                                                 \
            std::cerr << msg << "[" << __FILE__ << ":" << __LINE__ << "] "     \
                      << ""##__VA_ARGS__ << std::endl;                         \
            assert(expr);                                                      \
        }                                                                      \
    }
#endif // !NY_ASSERT_MSG

#pragma endregion assert

#pragma region memory

#ifndef NY_DELETE
#define NY_DELETE(pointer)                                                     \
    {                                                                          \
        delete pointer;                                                        \
        pointer = nullptr;                                                     \
    }
#endif // !NY_DELETE

#ifndef NY_DELETE_ARRAY
#define NY_DELETE_ARRAY(array_pointer)                                         \
    {                                                                          \
        delete[] pointer;                                                      \
        pointer = nullptr;                                                     \
    }
#endif // !NY_DELETE_ARRAY

#pragma endregion memory

#pragma region com

#ifndef NY_ASSERT_HR
#define NY_ASSERT_HR(x)                                                        \
    {                                                                          \
        HRESULT hr__ = (x);                                                    \
        NY_ASSERT(SUCCEEDED(hr__));                                            \
    }
#endif // NY_ASSERT_HR

#ifndef NY_ASSERT_HR_MSG
#define NY_ASSERT_HR_MSG(x, msg, ...)                                          \
    {                                                                          \
        HRESULT hr__ = (x);                                                    \
        NY_ASSERT_MSG(SUCCEEDED(hr__), msg, __VA_ARGS__);                      \
    }
#endif // NY_ASSERT_HR_MSG

#ifndef NY_RELEASE_COM
#define NY_RELEASE_COM(x)                                                      \
    {                                                                          \
        if (x) {                                                               \
            x->Release();                                                      \
            x = nullptr;                                                       \
        }                                                                      \
    }
#endif // NY_RELEASE_COM

#pragma endregion com

#pragma region NonConstMemFun Make member variable returned to can modify

#ifndef NON_CONST_MEM_FUN
#define NON_CONST_MEM_FUN(function)                                            \
    template <typename... TArgs>                                               \
    auto function(TArgs &&...args) ->                                          \
        typename std::remove_const_t<decltype(function(args...))> {            \
        return const_cast<decltype(function(args...))>(                        \
            std::as_const(*this).function(std::forward<TArgs>(args)...));      \
    }
#endif // !NON_CONST_MEM_FUN

#pragma endregion NonConstMemFun

#pragma region stringify

#ifndef STRINGIFY_INNER
#define STRINGIFY_INNER(Name) #Name
#endif
#ifndef STRINGIFY
#define STRINGIFY(Name) STRINGIFY_INNER(Name)
#endif

#pragma endregion stringify

#endif // !MACRO_H