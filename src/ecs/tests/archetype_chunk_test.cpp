#include "ecs/archetype/archetype_chunk.h"
#include "ecs/components/component_info.h"
#include "ecs/components/component.h"
#include "ecs/entity/entity.h"
#include <gtest/gtest.h>

class ArchetypeChunkTest : public ::testing::Test
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

        ComponentA() = default;

        ComponentA( ComponentA&& other )
        {
            // std::cout << "ComponentA move constructor called" << std::endl;
            a = other.a;
            b = other.b;
            c = std::move( other.c );
            k = other.k;
        }
    };

    struct ComponentB : public Component
    {
        i32 a = 0;
        f32 b = 3.3f;
        std::string c = "hello";
        i32& k;

        ComponentB( i32& k )
            : k( k )
        {
        }

        ComponentB( ComponentB&& other ) = default;

        virtual ~ComponentB() override
        {
            k = 0;
        }
    };
};

TEST_F( ArchetypeChunkTest, AddComponent )
{
    ArchetypeComponentMemoryInfo info;
    auto componentInfo = ComponentTypeRegistry::GetComponentInfo<ComponentA>();
    info.mComponentSize = componentInfo.size;
    info.mTotalSize = 0;
    info.mTotalOffset = 0;

    auto component = ComponentA();
    component.a = 23;
    component.b = 3.3f;
    component.c = "laksjdf";
    component.k = 12;

    Entity entity( 1, 1 );

    ArchetypeChunk archetypeChunk( 100 );
    archetypeChunk.AddEntity( entity, info );
    archetypeChunk.AddComponentData( entity, info, std::move( component ) );

    ComponentA* componentA = archetypeChunk.GetComponents<ComponentA>( info );

    EXPECT_EQ( componentA->a, 23 );
    EXPECT_EQ( componentA->b, 3.3f );
}

TEST_F( ArchetypeChunkTest, AddComponentWithOffset )
{
    ArchetypeComponentMemoryInfo info;
    auto componentInfo = ComponentTypeRegistry::GetComponentInfo<ComponentA>();
    info.mComponentSize = componentInfo.size;
    info.mTotalSize = 0;
    info.mTotalOffset = 0;

    ArchetypeChunk archetypeChunk( 100, 10 * 1024 );
    for ( Entity::IdType i = 0u; i < 12u; ++i )
    {
        Entity entity( i, 1 );
        archetypeChunk.AddEntity( entity, info );
    }

    Entity::IdType entityIndex = 11u;

    ComponentA component = ComponentA();
    component.a = 23;
    component.b = 3.3f;

    archetypeChunk.AddComponentData( Entity( entityIndex, 1u ), info, std::move( component ) );

    ComponentA* componentA = archetypeChunk.GetComponents<ComponentA>( info );

    componentA += entityIndex;
    EXPECT_EQ( componentA->a, 23 );
    EXPECT_EQ( componentA->b, 3.3f );
}

TEST_F( ArchetypeChunkTest, ReplaceComponent )
{
    ArchetypeChunk archetypeChunk( 100 );

    ArchetypeComponentMemoryInfo info;
    auto componentInfo = ComponentTypeRegistry::GetComponentInfo<ComponentB>();
    info.mComponentSize = componentInfo.size;
    info.mTotalSize = 0;
    info.mTotalOffset = 0;

    i32 k = 12;
    auto component = ComponentB( k );
    component.a = 23;
    component.b = 3.3f;

    Entity entity( 1, 1 );
    archetypeChunk.AddEntity( entity, info );
    archetypeChunk.AddComponentData( entity, info, std::move( component ) );

    i32 j = 0;
    auto componentB = ComponentB( j );
    componentB.a = 23;
    componentB.b = 3.3f;
    archetypeChunk.ReplaceComponentData( entity, info, std::move( componentB ) );

    ComponentB* componentBSeq = archetypeChunk.GetComponents<ComponentB>( info );
    ASSERT_EQ( componentBSeq->k, j );
}

TEST_F( ArchetypeChunkTest, DeconstructorComponent )
{
    ArchetypeChunk archetypeChunk( 100 );

    ArchetypeComponentMemoryInfo info;
    auto componentInfo = ComponentTypeRegistry::GetComponentInfo<ComponentB>();
    info.mComponentSize = componentInfo.size;
    info.mTotalSize = 0;
    info.mTotalOffset = 0;

    i32 k = 12;
    auto component = ComponentB( k );
    component.a = 23;
    component.b = 3.3f;

    Entity entity( 1, 1 );
    archetypeChunk.AddEntity( entity, info );
    archetypeChunk.AddComponentData( entity, info, std::move( component ) );

    ASSERT_EQ( k, 12 );

    i32 j = 0;
    auto componentB = ComponentB( j );
    componentB.a = 23;
    componentB.b = 3.3f;
    archetypeChunk.ReplaceComponentData( entity, info, std::move( componentB ) );

    ASSERT_EQ( k, 0 );
}
