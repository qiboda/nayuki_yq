#include <gtest/gtest.h>
#include <tuple>

#include "core/macro/macro.h"
#include "ecs/systems/params/local.h"
#include "ecs/systems/system_manager.h"
#include "ecs/systems/system_param.h"

int32_t Add( const int32_t a, const int32_t b )
{
    return a + b;
}

template <IsFnTrait AddFuncTrait>
void TestAddFuncTrait( FuncKind func_kind )
{
    using Arg0 = std::tuple_element_t<0, typename AddFuncTrait::ArgsTypeTuple>;
    using Arg1 = std::tuple_element_t<1, typename AddFuncTrait::ArgsTypeTuple>;

    constexpr bool ret = std::same_as<typename AddFuncTrait::ReturnType, i32>;
    constexpr size_t args_size = std::tuple_size_v<typename AddFuncTrait::ArgsTypeTuple>;

    // Note: The const declaration of parameters that are passed by value will be
    // implicitly removed in its signature.
    constexpr bool arg0 = std::same_as<Arg0, i32>;
    constexpr bool arg1 = std::same_as<Arg1, i32>;

    EXPECT_TRUE( ret );
    EXPECT_EQ( args_size, 2 );
    EXPECT_TRUE( arg0 );
    EXPECT_TRUE( arg1 );
    EXPECT_EQ( AddFuncTrait::FuncKind, func_kind );
}

TEST( FuncTraitTests, TestAddFuncTrait )
{
    TestAddFuncTrait<FnTrait<decltype( Add )>>( FuncKind::Func );
    TestAddFuncTrait<FnTrait<decltype( &Add )>>( FuncKind::FuncPtr );
    TestAddFuncTrait<FnTrait<decltype( Add )&>>( FuncKind::FuncRef );
}

class SystemTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

  public:
    static void add( i32 a, i32 b )
    {
        i32 c = a + b;
        UNUSED_VAR( c );
    }

    static void addvoid()
    {
    }

    static i32 addvoidreturn()
    {
        return 0;
    }

    static void test_local( Local<i32>& a )
    {
        a.mValue++;
    }
};

TEST_F( SystemTest, IsSystemTest )
{
    bool system1 = IsSystem<decltype( &SystemTest::add )>;
    EXPECT_FALSE( system1 );
    bool system2 = IsSystem<decltype( &SystemTest::addvoid )>;
    EXPECT_FALSE( system2 );
    bool system3 = AllSystemParamsConcept<>;
    EXPECT_TRUE( system3 );
    bool system4 = IsSystem<decltype( &SystemTest::addvoidreturn )>;
    EXPECT_FALSE( system4 );
    bool localSystemParam = IsSystemParamConcept<Local<i32>>;
    EXPECT_TRUE( localSystemParam );
    bool localSystemParams = AllSystemParamsConcept<Local<i32>>;
    EXPECT_TRUE( localSystemParams );
    bool system5 = IsSystem<decltype( &SystemTest::test_local )>;
    EXPECT_TRUE( system5 );
}

TEST_F( SystemTest, LocalParamTest )
{
    Registry registry;
    auto sm = SystemManager();
    SystemId id = sm.AddSystem( &SystemTest::test_local );

    {
        sm.RunSystem( &registry );
        const auto& system = sm.GetSystem( id );
        const auto& systemDc = system->Downcast( &SystemTest::test_local );
        const auto& systemState = systemDc->GetSystemState();
        auto paramState = systemState.GetParamState<0>();
        EXPECT_EQ( paramState.value, 1 );
    }

    {
        sm.RunSystem( &registry );
        const auto& system = sm.GetSystem( id );
        const auto& systemDc = system->Downcast( &SystemTest::test_local );
        const auto& systemState = systemDc->GetSystemState();
        auto paramState = systemState.GetParamState<0>();
        EXPECT_EQ( paramState.value, 2 );
    }
}