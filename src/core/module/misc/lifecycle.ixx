module;

#include <module_export.h>

export module core.misc.lifecycle;

import std;

template <typename T, typename... Args>
concept HasOnInit = requires( T t, Args&&... args ) {
    { t.OnInit( std::forward<Args>( args )... ) };
};

template <typename T, typename... Args>
concept HasClear = requires( T t, Args&&... args ) {
    { t.Clear( std::forward<Args>( args )... ) };
};

/**
 * @brief
 * 不支持多层级继承。
 * @tparam Derived
 */
template <typename Derived>
class Lifecycle
{
  public:
    template <typename... Args>
        requires HasOnInit<Derived, Args...>
    void Init( Args&&... args )
    {
        static_cast<Derived*>( this )->OnInit( std::forward<Args>( args )... );
    }

    template <typename... Args>
        requires HasClear<Derived, Args...>
    void Clear( Args&&... args )
    {
        static_cast<Derived*>( this )->Clear( std::forward<Args>( args )... );
    }
};