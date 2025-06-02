#pragma once

#include "core/concepts/are_unique.h"
#include "ecs/components/component.h"
#include "ecs/entity/entity.h"

// 不强制要求 is_trivially_copyable_v
template <typename T>
concept IsComponentConcept = IsStrictDerivedConcept<Component, T>;
//  && std::is_trivially_copyable_v<T>;

template <typename T>
concept IsRegularComponentConcept = IsStrictDerivedConcept<Component, T> && ( IsEntityConcept<T> == false );

template <typename T>
concept IsEntityComponentConcept = IsStrictDerivedConcept<Component, T> && IsEntityConcept<T>;

template <typename... T>
concept IsComponentSetConcept = ( IsComponentConcept<T> && ... ) && AreUniqueValue<T...>;

template <typename... T>
concept IsRegularComponentSetConcept = ( IsRegularComponentConcept<T> && ... ) && AreUniqueValue<T...>;

enum class ComponentKind : u8
{
    Invalid,
    Regular,
    Entity,
};

template <IsComponentConcept T>
struct ComponentBaseTrait
{
    // 已经是对齐后的 size
    static constexpr size_t Size = sizeof( T );
    static constexpr size_t Align = alignof( T );
};

template <typename T>
struct ComponentTrait : ComponentBaseTrait<T>
{
    inline static constexpr ComponentKind Kind = ComponentKind::Invalid;
};

template <IsEntityComponentConcept T>
struct ComponentTrait<T> : ComponentBaseTrait<T>
{
    inline static constexpr ComponentKind Kind = ComponentKind::Entity;
};

template <IsRegularComponentConcept T>
struct ComponentTrait<T> : ComponentBaseTrait<T>
{
    inline static constexpr ComponentKind Kind = ComponentKind::Entity;
};