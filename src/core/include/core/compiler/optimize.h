#pragma once

#if defined( _MSC_VER )
#    define NO_OPTIMIZE_BEGIN __pragma( optimize( "", off ) )
#    define NO_OPTIMIZE_END __pragma( optimize( "", on ) )
// #elif defined( __GNUC__ )
// #    define NO_OPTIMIZE_BEGIN _Pragma( "GCC push_options" ) _Pragma( "GCC optimize(\"O0\")" )
// #    define NO_OPTIMIZE_END _Pragma( "GCC pop_options" )
#elif defined( __clang__ )
#    define NO_OPTIMIZE_BEGIN _Pragma( "clang optimize off" )
#    define NO_OPTIMIZE_END _Pragma( "clang optimize on" )
#else
#    define NO_OPTIMIZE_BEGIN
#    define NO_OPTIMIZE_END
#endif