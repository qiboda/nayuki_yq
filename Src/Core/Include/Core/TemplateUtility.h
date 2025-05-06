#pragma once


#pragma region is_shared_ptr

template <typename T> struct is_shared_ptr : std::false_type {};

template <typename T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template <typename T>
inline constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

#pragma endregion is_shared_ptr

#pragma region non_const

/** non-const error */
template <typename T> struct non_const;

/** handle top const */
template <typename T> struct non_const<const T> {
    using type = T;
};

/** handle bottom const */
template <typename T> struct non_const<const T *> {
    using type = T *;
};

template <typename T> using non_const_t = typename non_const<T>::type;

#pragma endregion non_const