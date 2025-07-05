module;

#include <module_export.h>
#include <core/macro/flag_enum.h>

export module meta.user_type.define;

import std;
import core;
import core.misc.flag_enum;

export enum class QualifierFlag : u8 {
    None = 0,
    Const = 1 << 0,     // const T
    Volatile = 1 << 1,  // volatile T
    Inline = 1 << 2,    // inline T
    Constexpr = 1 << 3, // constexpr T
    Static = 1 << 4,    // constexpr T
};
FLAG_ENUM( QualifierFlag );

/**
 * @brief T*, T&, T&&, T[]等复合类型。
 * TypeBase 内存储复合类型的信息。
 * 原始类型的信息存储在原始类型中
 */
export enum class CompoundFlag : u8 {
    None = 0,
    Pointer = 1 << 0,         // T*
    Reference = 1 << 1,       // T&
    RValueReference = 1 << 2, // T&&
    RawArray = 1 << 3,        // T[]
    // Function = 1 << 4,       // T()
    // MemberFunction = 1 << 5, // T::()
    // Method = 1 << 6,         // T::T()
};
FLAG_ENUM( CompoundFlag );

export enum class AccessLevel : u8 {
    Public,
    Protected,
    Private,
};
