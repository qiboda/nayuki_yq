#include "ecs/archetype/archetype.h"
#include <gtest/gtest.h>
#include <ecs/archetype/archetype_chunk.h>
#include <ecs/components/component.h>
#include <ecs/entity/entity.h>
#include <ecs/entity/entity_manager.h>

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