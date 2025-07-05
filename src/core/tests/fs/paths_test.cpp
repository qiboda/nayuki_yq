#include <gtest/gtest.h>

import std;
import core;
import core.fs.paths;
import core.platform.exe;

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
