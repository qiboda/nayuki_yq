#include "ecs/components/component.h"
#include "ecs/components/component_info.h"
#include <gtest/gtest.h>

class ComponentIdSetTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

    class ComponentA : public Component
    {
    };

    class ComponentB : public Component
    {
    };

    class ComponentC : public Component
    {
    };
};

TEST_F( ComponentIdSetTest, IncludeDiffComponentsTest )
{
    ComponentIdSet set1 = ComponentTypeRegistry::GetComponentIdSet<ComponentA, ComponentB, ComponentC>();
    ComponentIdSet set2 = ComponentTypeRegistry::GetComponentIdSet<ComponentA, ComponentB>();

    EXPECT_TRUE( set1.Include( set2 ) );
    EXPECT_FALSE( set2.Include( set1 ) );
}

TEST_F( ComponentIdSetTest, IncludeSameComponentsTest )
{
    ComponentIdSet set1 = ComponentTypeRegistry::GetComponentIdSet<ComponentA, ComponentB>();
    ComponentIdSet set2 = ComponentTypeRegistry::GetComponentIdSet<ComponentA, ComponentB>();

    EXPECT_TRUE( set1.Include( set2 ) );
    EXPECT_TRUE( set2.Include( set1 ) );
}

TEST_F( ComponentIdSetTest, IncludeNoOrderComponentsTest )
{
    ComponentIdSet set1 = ComponentTypeRegistry::GetComponentIdSet<ComponentA, ComponentB>();
    ComponentIdSet set2 = ComponentTypeRegistry::GetComponentIdSet<ComponentB, ComponentA>();

    EXPECT_TRUE( set1.Include( set2 ) );
    EXPECT_TRUE( set2.Include( set1 ) );
}

TEST_F( ComponentIdSetTest, IncludeNoOrderDiffComponentsTest )
{
    ComponentIdSet set1 = ComponentTypeRegistry::GetComponentIdSet<ComponentA, ComponentB>();
    ComponentIdSet set2 = ComponentTypeRegistry::GetComponentIdSet<ComponentC, ComponentB, ComponentA>();

    EXPECT_FALSE( set1.Include( set2 ) );
    EXPECT_TRUE( set2.Include( set1 ) );
}