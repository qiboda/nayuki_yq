#include <gtest/gtest.h>

#include <meta/minimal.h>
#include <meta/type_id.h>

class TypeIdTest : public ::testing::Test
{
  protected:
    TypeIdTest()
    {
    }

    ~TypeIdTest() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F( TypeIdTest, TypeIdGeneratorGet )
{
    TypeId id1 = TypeIdGenerator::Get<int>();
    TypeId id2 = TypeIdGenerator::Get<float>();
    TypeId id3 = TypeIdGenerator::Get<double>();

    ASSERT_NE( id1, id2 );
    ASSERT_NE( id1, id3 );
    ASSERT_NE( id2, id3 );

    // 再次获取相同类型的TypeId应该是相同的
    TypeId id4 = TypeIdGenerator::Get<int>();
    ASSERT_EQ( id1, id4 );
}

TEST_F( TypeIdTest, TypeIdGeneratorGetWithConst )
{
    TypeId id1 = TypeIdGenerator::Get<const int>();
    TypeId id2 = TypeIdGenerator::Get<int>();
    ASSERT_EQ( id1, id2 ); // const int 和 int 应该是相同的 TypeId
}

namespace _TypeIdTestDetail
{
class A
{
};
}; // namespace _TypeIdTestDetail

namespace _TypeIdTestDetail2
{
class A
{
};
}; // namespace _TypeIdTestDetail2

TEST_F( TypeIdTest, NamespaceSameTypeNameTest )
{
    TypeId id1 = TypeIdGenerator::Get<_TypeIdTestDetail::A>();
    TypeId id2 = TypeIdGenerator::Get<_TypeIdTestDetail2::A>();
    ASSERT_NE( id1, id2 );
}