#include <gtest/gtest.h>

import ecs.components.component;
import ecs.components.info;
import ecs.archetype.chunk;
import ecs.archetype.archetype;
import ecs.entity.entity;
import ecs.entity.manager;
import core.type;

class ArchetypeTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // This function will be called before each test.
    }

    void TearDown() override
    {
        // This function will be called after each test.
    }

    struct ComponentA : public Component
    {
        i32 a = 0;
        f32 b = 3.3f;
        std::string c = "hello";
        i32 k;
    };

    struct ComponentB : public Component
    {
        i32 a = 0;
        f32 b = 3.3f;
    };
};

TEST_F( ArchetypeTest, test_name )
{
    ComponentIdSet componentIdSet;
    componentIdSet.Add( ComponentTypeRegistry::Get<ComponentA>() );
    componentIdSet.Add( ComponentTypeRegistry::Get<ComponentB>() );

    ComponentTypeRegistry::GetComponentInfo<ComponentA>();
    ComponentTypeRegistry::GetComponentInfo<ComponentB>();

    Archetype archetype;
    archetype.Init( std::move( componentIdSet ) );
}