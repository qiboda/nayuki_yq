
#pragma once

#include <clang/Tooling/CommonOptionsParser.h>
#include <clang/Tooling/Tooling.h>
#include <clang/AST/AST.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RecordLayout.h>

#include <cstddef>

/**
 * todo: 也共享一份到 core
 */
template <typename T, typename... Rest>
void hash_combine( std::size_t& seed, const T& v, const Rest&... rest )
{
    seed ^= std::hash<T>{}( v ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
    ( hash_combine( seed, rest ), ... );
}
