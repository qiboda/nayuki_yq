#include <gtest/gtest.h>

#include <Core/Core.h>
#include <Core/TemplateUtility.h>

class TemplateUtilityTest
    : public ::testing::Test
{
protected:
    TemplateUtilityTest() {}
    ~TemplateUtilityTest() override {}

    void SetUp() override {}
    void TearDown() override {}
};

TEST_F( TemplateUtilityTest, IsSharedPtr )
{
    std::shared_ptr<int> sp = std::make_shared<int>();
    ASSERT_TRUE( is_shared_ptr<decltype( sp )>::value );
    ASSERT_TRUE( is_shared_ptr_v<decltype( sp )> );
    ASSERT_FALSE( is_shared_ptr<int*>::value );
    ASSERT_FALSE( is_shared_ptr_v<std::weak_ptr<double> > );
}