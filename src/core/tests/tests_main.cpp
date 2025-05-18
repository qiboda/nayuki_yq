
#include <gtest/gtest.h>
#include <core/memory/global_new.h>

int main( int argc, char **argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}