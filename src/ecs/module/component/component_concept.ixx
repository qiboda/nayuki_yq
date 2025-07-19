export module ecs:component_concepts;

import core;
import core.concepts.derived;
import core.concepts.are_unique;
import :component;
import :entity;

// 不强制要求 is_trivially_copyable_v
export template <typename T>
concept IsComponentConcept = IsStrictDerivedConcept<Component, T>;
//  && std::is_trivially_copyable_v<T>;

export template <typename T>
concept IsRegularComponentConcept = IsStrictDerivedConcept<Component, T> && ( IsEntityConcept<T> == false );

export template <typename T>
concept IsEntityComponentConcept = IsStrictDerivedConcept<Component, T> && IsEntityConcept<T>;

export template <typename... T>
concept IsComponentSetConcept = ( IsComponentConcept<T> && ... ) && AreUniqueValue<T...>;

export template <typename... T>
concept IsRegularComponentSetConcept = ( IsRegularComponentConcept<T> && ... ) && AreUniqueValue<T...>;

export enum class ComponentKind : u8 {
    Invalid,
    Regular,
    Entity,
};

export template <IsComponentConcept T>
struct ComponentBaseTrait
{
    // 已经是对齐后的 size
    static constexpr usize Size = sizeof( T );
    static constexpr usize Align = alignof( T );
};

export template <typename T>
struct ComponentTrait : ComponentBaseTrait<T>
{
    inline static constexpr ComponentKind Kind = ComponentKind::Invalid;
};

export template <IsEntityComponentConcept T>
struct ComponentTrait<T> : ComponentBaseTrait<T>
{
    inline static constexpr ComponentKind Kind = ComponentKind::Entity;
};

export template <IsRegularComponentConcept T>
struct ComponentTrait<T> : ComponentBaseTrait<T>
{
    inline static constexpr ComponentKind Kind = ComponentKind::Regular;
};