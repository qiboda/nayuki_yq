#include <gtest/gtest.h>

#include "ecs/components/component.h"
#include "ecs/entity/entity.h"
#include "ecs/systems/query.h"
#include "ecs/systems/query_param.h"

class QueryTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
        // Clean up
    }

    struct ComponentA : public Component
    {
        i32 i;
    };

    struct ComponentB : public Component
    {
        i32 i;
    };

    struct ComponentC : public Component
    {
        i32 i;
    };
};

TEST_F( QueryTest, QueryParamComponentQualifier )
{
    auto query1 = IsQueryParamSetConcept<ComponentA>;
    EXPECT_FALSE( query1 );
    auto query2 = IsQueryParamSetConcept<const ComponentA>;
    EXPECT_FALSE( query2 );
    auto query3 = IsQueryParamSetConcept<ComponentA &>;
    EXPECT_TRUE( query3 );
    auto query4 = IsQueryParamSetConcept<const ComponentA &>;
    EXPECT_TRUE( query4 );
    auto query5 = IsQueryParamSetConcept<ComponentB *>;
    EXPECT_FALSE( query5 );
    auto query6 = IsQueryParamSetConcept<const ComponentA *>;
    EXPECT_FALSE( query6 );
}

TEST_F( QueryTest, QueryParamEntityQualifier )
{
    auto IsEntityParam = IsEntityConcept<std::decay_t<Entity>>;
    EXPECT_TRUE( IsEntityParam );
    auto EntityParamLimit = EntityParamLimitConcept<Entity>;
    EXPECT_TRUE( EntityParamLimit );

    auto query1 = IsQueryParamSetConcept<Entity>;
    EXPECT_TRUE( query1 );
    auto query2 = IsQueryParamSetConcept<const Entity>;
    EXPECT_FALSE( query2 );
    auto query3 = IsQueryParamSetConcept<Entity *>;
    EXPECT_FALSE( query3 );
    auto query4 = IsQueryParamSetConcept<const Entity *>;
    EXPECT_FALSE( query4 );
    auto query5 = IsQueryParamSetConcept<Entity &>;
    EXPECT_FALSE( query5 );
    auto query6 = IsQueryParamSetConcept<const Entity &>;
    EXPECT_FALSE( query6 );
}

TEST_F( QueryTest, QueryParamSameType )
{
    auto query1 = IsQueryParamSetConcept<ComponentA &, ComponentB &>;
    EXPECT_EQ( query1, true );
    auto query2 = IsQueryParamSetConcept<ComponentA &, ComponentA &>;
    EXPECT_EQ( query2, false );
    auto query3 = IsQueryParamSetConcept<ComponentA &, ComponentB &, const ComponentA &>;
    EXPECT_EQ( query3, false );
    auto value4 = IsQueryParamSetConcept<ComponentA &, Entity>;
    EXPECT_EQ( value4, true );
    auto value5 = IsQueryParamSetConcept<ComponentA &, Entity, Entity>;
    EXPECT_EQ( value5, false );
}