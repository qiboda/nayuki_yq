#include <gtest/gtest.h>
#include <tuple>

#include "core/macro/macro.h"
#include "ecs/systems/system.h"
#include "ecs/systems/system_param.h"

int32_t Add( const int32_t a, const int32_t b )
{
    return a + b;
}

template <IsFnTrait AddFuncTrait>
void TestAddFuncTrait( FuncType func_type )
{
    using Arg0 = std::tuple_element_t<0, typename AddFuncTrait::ArgsTypeList>;
    using Arg1 = std::tuple_element_t<1, typename AddFuncTrait::ArgsTypeList>;

    constexpr bool ret = std::same_as<typename AddFuncTrait::ReturnType, i32>;
    constexpr size_t args_size = std::tuple_size_v<typename AddFuncTrait::ArgsTypeList>;

    // Note: The const declaration of parameters that are passed by value will be
    // implicitly removed in its signature.
    constexpr bool arg0 = std::same_as<Arg0, i32>;
    constexpr bool arg1 = std::same_as<Arg1, i32>;

    EXPECT_TRUE( ret );
    EXPECT_EQ( args_size, 2 );
    EXPECT_TRUE( arg0 );
    EXPECT_TRUE( arg1 );
    EXPECT_EQ( AddFuncTrait::FuncType, func_type );
}

TEST( FuncTraitTests, TestAddFuncTrait )
{
    TestAddFuncTrait<FnTrait<decltype( Add )>>( FuncType::Func );
    TestAddFuncTrait<FnTrait<decltype( &Add )>>( FuncType::FuncPtr );
    TestAddFuncTrait<FnTrait<decltype( Add ) &>>( FuncType::FuncRef );
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
};

TEST_F( SystemTest, SystemTest )
{
    bool system1 = IsSystem<decltype( &SystemTest::add )>;
    EXPECT_FALSE( system1 );
    bool system2 = IsSystem<decltype( &SystemTest::addvoid )>;
    EXPECT_TRUE( system2 );
    bool system3 = IsSystemParamSetConcept<>;
    EXPECT_TRUE( system3 );
    bool system4 = IsSystem<decltype( &SystemTest::addvoidreturn )>;
    EXPECT_FALSE( system4 );
}