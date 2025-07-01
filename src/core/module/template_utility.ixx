export module core.template_utility;

import std;

#pragma region is_shared_ptr

export template <typename T>
struct is_shared_ptr : std::false_type
{
};

export template <typename T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type
{
};

export template <typename T>
inline constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

#pragma endregion is_shared_ptr

#pragma region non_const

/** non-const error */
export template <typename T>
struct non_const;

/** handle top const */
export template <typename T>
struct non_const<const T>
{
    using type = T;
};

/** handle bottom const */
export template <typename T>
struct non_const<const T*>
{
    using type = T*;
};

export template <typename T>
using non_const_t = typename non_const<T>::type;

#pragma endregion non_const