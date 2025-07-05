
#include <gtest/gtest.h>
#include <core/global_new.h>

int main( int argc, char** argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
