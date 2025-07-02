export module core.delegate:interface;

import :delegate;

export template <typename... TArgs>
using MultipleDelegateDecl = MultipleDelegate<void, TArgs...>;

export template <typename... TArgs>
using SingleDelegateDecl = SingleDelegate<void, TArgs...>;

export template <typename TRet, typename... TArgs>
using SingleRetDelegateDecl = SingleDelegate<TRet, TArgs...>;