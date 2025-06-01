#include <gtest/gtest.h>
#include <tuple>

#include "core/macro/macro.h"
#include "ecs/commands/entity_command.h"
#include "ecs/commands/entity_instance_command.h"
#include "ecs/components/component.h"
#include "ecs/commands/registry_command.h"
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
    auto query1 = IsQueryArgsSetConcept<ComponentA>;
    EXPECT_FALSE( query1 );
    auto query2 = IsQueryArgsSetConcept<const ComponentA>;
    EXPECT_FALSE( query2 );
    auto query3 = IsQueryArgsSetConcept<ComponentA&>;
    EXPECT_TRUE( query3 );
    auto query4 = IsQueryArgsSetConcept<const ComponentA&>;
    EXPECT_TRUE( query4 );
    auto query5 = IsQueryArgsSetConcept<ComponentB*>;
    EXPECT_FALSE( query5 );
    auto query6 = IsQueryArgsSetConcept<const ComponentA*>;
    EXPECT_FALSE( query6 );
}

TEST_F( QueryTest, QueryParamEntityQualifier )
{
    auto IsEntityParam = IsEntityConcept<std::decay_t<Entity>>;
    EXPECT_TRUE( IsEntityParam );
    auto EntityParamLimit = EntityArgsLimitConcept<Entity>;
    EXPECT_TRUE( EntityParamLimit );

    auto query1 = IsQueryArgsSetConcept<Entity>;
    EXPECT_TRUE( query1 );
    auto query2 = IsQueryArgsSetConcept<const Entity>;
    EXPECT_FALSE( query2 );
    auto query3 = IsQueryArgsSetConcept<Entity*>;
    EXPECT_FALSE( query3 );
    auto query4 = IsQueryArgsSetConcept<const Entity*>;
    EXPECT_FALSE( query4 );
    auto query5 = IsQueryArgsSetConcept<Entity&>;
    EXPECT_FALSE( query5 );
    auto query6 = IsQueryArgsSetConcept<const Entity&>;
    EXPECT_FALSE( query6 );
}

TEST_F( QueryTest, QueryParamSameType )
{
    auto query1 = IsQueryArgsSetConcept<ComponentA&, ComponentB&>;
    EXPECT_EQ( query1, true );
    auto query2 = IsQueryArgsSetConcept<ComponentA&, ComponentA&>;
    EXPECT_EQ( query2, false );
    auto query3 = IsQueryArgsSetConcept<ComponentA&, ComponentB&, const ComponentA&>;
    EXPECT_EQ( query3, false );
    auto value4 = IsQueryArgsSetConcept<ComponentA&, Entity>;
    EXPECT_EQ( value4, true );
    auto value5 = IsQueryArgsSetConcept<ComponentA&, Entity, Entity>;
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
    Query<ComponentA&> query = Query<ComponentA&>::From( &registry, queryState );

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