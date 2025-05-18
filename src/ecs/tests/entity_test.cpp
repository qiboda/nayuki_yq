#include <gtest/gtest.h>

#include <ecs/entity.h>

class EntityTest : public ::testing::Test
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
};

TEST_F( EntityTest, DefaultConstructor )
{
    BasicEntity<u32> entity;
    EXPECT_EQ( entity.GetId(), 0 );
    EXPECT_EQ( entity.GetVersion(), 0 );
}

TEST_F( EntityTest, ParameterizedConstructor )
{
    BasicEntity<u32> entity( ( 12345u << 12u ) + 1234u );
    EXPECT_EQ( entity.GetId(), 12345 );
    EXPECT_EQ( entity.GetVersion(), 1234 );
}

TEST_F( EntityTest, IdOffset )
{
    EXPECT_EQ( BasicEntity<u32>::id_offset, 12 );
    EXPECT_EQ( BasicEntity<u64>::id_offset, 24 );
}
