#include <gtest/gtest.h>

#include <Core/Macro/Macro.h>
#include <Core/TypeDef.h>


class MacroTest : public ::testing::Test {
  protected:
    MacroTest() {}
    ~MacroTest() override {}

    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(MacroTest, AssertPrint) {
    ASSERT_EXIT(
        NY_ASSERT_MSG((3 == 4), "error: "),
        [](i32 code) -> bool {
            UNUSED_VAR(code);
            return true;
        },
        "");
    ASSERT_DEATH(NY_ASSERT_MSG((3 == 4), "error: "), "");
}