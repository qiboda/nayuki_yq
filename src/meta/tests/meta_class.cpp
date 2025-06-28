
#include "gtest/gtest.h"
#include <meta/meta_registry.h>

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
    class A
    {
      public:
        static void MetaRegistry();

        constexpr void TestMethod()
        {
        }
    };
};

void TypeClassRegistryTest::A::MetaRegistry()
{
    MetaRegistry::Class<A>( "MyClass", Namespace::Parse( "ns" ) ).AddMethod<&A::TestMethod>( "TestMethod" );
}

TEST( TypeClassRegistryTest, TestClassRegistry )
{
    TypeClassRegistryTest::A::MetaRegistry();
    std::weak_ptr<Class> cls = TypeManager::GetInstance().GetClass<TypeClassRegistryTest::A>();
    EXPECT_FALSE( cls.expired() );
}