
#include <gtest/gtest.h>
#include <core/global_new.h>

int main( int argc, char **argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    GTEST_FLAG_SET( death_test_style, "threadsafe" );
    return RUN_ALL_TESTS();
}