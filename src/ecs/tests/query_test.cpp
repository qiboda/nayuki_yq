#include <gtest/gtest.h>
#include <tuple>

#include "ecs/commands/entity_command.h"
#include "ecs/commands/entity_instance_command.h"
#include "ecs/components/component.h"
#include "ecs/commands/registry_command.h"
#include "ecs/entity/entity.h"
#include "ecs/systems/params/query/query.h"
#include "ecs/systems/params/query/query_data.h"

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

    void QueryComponentA( Query<ComponentA&>& query )
    {
        for ( auto [component] : query )
        {
            component.i += 1;
        }
    }
};

TEST_F( QueryTest, QueryParamComponentQualifier )
{
    auto query1 = IsQueryDataArgConcept<ComponentA>;
    EXPECT_FALSE( query1 );
    auto query2 = IsQueryDataArgConcept<const ComponentA>;
    EXPECT_FALSE( query2 );
    auto query3 = IsQueryDataArgConcept<ComponentA&>;
    EXPECT_TRUE( query3 );
    auto query4 = IsQueryDataArgConcept<const ComponentA&>;
    EXPECT_TRUE( query4 );
    auto query5 = IsQueryDataArgConcept<ComponentB*>;
    EXPECT_FALSE( query5 );
    auto query6 = IsQueryDataArgConcept<const ComponentA*>;
    EXPECT_FALSE( query6 );
}

TEST_F( QueryTest, QueryParamEntityQualifier )
{
    auto IsEntityParam = IsEntityConcept<std::decay_t<Entity>>;
    EXPECT_TRUE( IsEntityParam );
    auto EntityParamLimit = EntityArgsLimitConcept<Entity>;
    EXPECT_TRUE( EntityParamLimit );

    auto query1 = IsQueryDataArgConcept<Entity>;
    EXPECT_TRUE( query1 );
    auto query2 = IsQueryDataArgConcept<const Entity>;
    EXPECT_FALSE( query2 );
    auto query3 = IsQueryDataArgConcept<Entity*>;
    EXPECT_FALSE( query3 );
    auto query4 = IsQueryDataArgConcept<const Entity*>;
    EXPECT_FALSE( query4 );
    auto query5 = IsQueryDataArgConcept<Entity&>;
    EXPECT_FALSE( query5 );
    auto query6 = IsQueryDataArgConcept<const Entity&>;
    EXPECT_FALSE( query6 );
}

TEST_F( QueryTest, QueryParamSameType )
{
    auto query1 = IsQueryParamsValue<ComponentA&, ComponentB&>;
    EXPECT_EQ( query1, true );
    auto query2 = IsQueryParamsValue<ComponentA&, ComponentA&>;
    EXPECT_EQ( query2, false );
    auto query3 = IsQueryParamsValue<ComponentA&, ComponentB&, const ComponentA&>;
    EXPECT_EQ( query3, false );
    auto value4 = IsQueryParamsValue<ComponentA&, Entity>;
    EXPECT_EQ( value4, true );
    auto value5 = IsQueryParamsValue<ComponentA&, Entity, Entity>;
    EXPECT_EQ( value5, false );
}

TEST_F( QueryTest, QuerySingleComponent )
{
    Registry registry;

    ComponentTypeRegistry::Register<ComponentA>();

    auto componentA = ComponentA();
    componentA.i = 3;

    // clang-format off
    RegistryCommand::Get( &registry )
        ->Entity()
            ->Create()
                ->AddComponent( std::move( componentA ) );
    // clang-format on

    registry.mCommandManager->ExecuteCommands( &registry );

    QueryState<ComponentA&> queryState;
    queryState.Init( &registry );
    Query<ComponentA&> query = Query<ComponentA&>::From( &registry, &queryState );

    usize count = 0;
    for ( auto [a] : query )
    {
        a.i += 1;
        count += 1;
        EXPECT_EQ( a.i, 4 );
    }
    EXPECT_EQ( count, 1 );

    for ( auto [a] : query )
    {
        a.i += 1;
        count += 1;
        EXPECT_EQ( a.i, 5 );
    }
    EXPECT_EQ( count, 2 );
}

TEST_F( QueryTest, QueryReadWrite )
{
    using ABCTrait = SystemParamTrait<Query<ComponentA&, const ComponentB&, ComponentC&>>;
    EXPECT_EQ( ABCTrait::ComponentsReadWrite.size(), 3 );

    EXPECT_EQ( ABCTrait::ComponentsReadWrite[0].second, true );
    EXPECT_EQ( ABCTrait::ComponentsReadWrite[1].second, false );
    EXPECT_EQ( ABCTrait::ComponentsReadWrite[2].second, true );
}

TEST_F( QueryTest, StructuredBindingTest )
{
    const f32 v = 3.0;
    bool s = false;
    i32 i = 3;

    std::tuple<i32&, bool&, const f32&> k = std::forward_as_tuple( i, s, v );
    auto [a, b, c] = k;
    a += 3;
    b = true;

    EXPECT_TRUE( std::get<1>( k ) );
    EXPECT_EQ( std::get<0>( k ), 6 );
    // c = 4.0f; // 报错

    auto [d, e, f] = k;
    d += 3;
    e = false;

    EXPECT_FALSE( std::get<1>( k ) );
    EXPECT_EQ( std::get<0>( k ), 9 );
}