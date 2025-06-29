#include <gtest/gtest.h>

#include <core/core.h>
#include <core/macro/macro.h>
#include <core/type_def.h>
#include <core/fs/paths.h>
#include <core/platform/platform_exe.h>

class PathsTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F( PathsTest, HasRootPath )
{
    FsPath engineFolder = Paths::EngineFolder();
    FsPath nayukiYqFile = engineFolder / "nayuki.yq";
    EXPECT_TRUE( std::filesystem::exists( nayukiYqFile ) );
}
