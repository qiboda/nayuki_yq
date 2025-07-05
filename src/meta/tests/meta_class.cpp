
#include "gtest/gtest.h"

import std;
import meta.meta_registry;
import meta.type_manager;
import meta.user_type.ns;
import meta.user_type.class_type;

class TypeClassRegistryTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

  public:
};

class A
{
  public:
    static void MetaRegistry();

    constexpr void TestMethod()
    {
    }
};

void A::MetaRegistry()
{
    MetaRegistry::Class<A>( "MyClass", Namespace::Parse( "ns" ) ).AddMethod<&A::TestMethod>( "TestMethod" );
}

TEST( TypeClassRegistryTest, TestClassRegistry )
{
    A::MetaRegistry();
    std::weak_ptr<Class> cls = TypeManager::GetInstance().GetClass<A>();
    EXPECT_FALSE( cls.expired() );
}