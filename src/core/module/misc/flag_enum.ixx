export module core.misc.flag_enum;

import std;

export template <typename E>
struct IsFlagEnum : std::false_type
{
};

export template <typename E>
    requires std::is_enum_v<E>
inline constexpr bool IsFlagEnumValue = IsFlagEnum<E>::value;

export template <typename E>
concept IsFlagEnumConcept = IsFlagEnumValue<E>;

// 位或
template <IsFlagEnumConcept E>
constexpr E operator|( E lhs, E rhs )
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>( static_cast<U>( lhs ) | static_cast<U>( rhs ) );
}

// 位与
template <IsFlagEnumConcept E>
constexpr E operator&( E lhs, E rhs )
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>( static_cast<U>( lhs ) & static_cast<U>( rhs ) );
}

// 位异或
template <IsFlagEnumConcept E>
constexpr E operator^( E lhs, E rhs )
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>( static_cast<U>( lhs ) ^ static_cast<U>( rhs ) );
}

// 取反
template <IsFlagEnumConcept E>
constexpr E operator~( E e )
{
    using U = std::underlying_type_t<E>;
    return static_cast<E>( ~static_cast<U>( e ) );
}

// 或等
template <IsFlagEnumConcept E>
constexpr E& operator|=( E& lhs, E rhs )
{
    lhs = lhs | rhs;
    return lhs;
}

// 与等
template <IsFlagEnumConcept E>
constexpr E& operator&=( E& lhs, E rhs )
{
    lhs = lhs & rhs;
    return lhs;
}

// 异或等
template <IsFlagEnumConcept E>
constexpr E& operator^=( E& lhs, E rhs )
{
    lhs = lhs ^ rhs;
    return lhs;
}