/// NOTE
/// 每个 DLL 必须引入这个头文件，才能使用全局替换的new和delete。
/// 因为这个重定义不能跨越 DLL 模块。
/// 按照项目规范，放置在模块对应的源文件中，例如core模块，就是在core.cpp中。
#pragma once

#include <cstdio>
#include <new>

extern void* operator new( std::size_t size ) noexcept( false );

extern void* operator new[]( std::size_t size ) noexcept( false );

extern void* operator new( std::size_t size, const std::nothrow_t& tag ) noexcept;

extern void* operator new[]( std::size_t size, const std::nothrow_t& tag ) noexcept;

extern void* operator new( std::size_t size, std::align_val_t align ) noexcept( false );

extern void* operator new[]( std::size_t size, std::align_val_t align ) noexcept( false );

extern void* operator new( std::size_t size, std::align_val_t align, const std::nothrow_t& tag ) noexcept;

extern void* operator new[]( std::size_t size, std::align_val_t align, const std::nothrow_t& tag ) noexcept;

extern void operator delete( void* p ) noexcept;

extern void operator delete[]( void* p ) noexcept;

extern void operator delete( void* p, std::size_t size ) noexcept;

extern void operator delete[]( void* p, std::size_t size ) noexcept;

extern void operator delete( void* p, std::align_val_t align ) noexcept;

extern void operator delete[]( void* p, std::align_val_t align ) noexcept;

extern void operator delete( void* p, std::size_t size, std::align_val_t align ) noexcept;

extern void operator delete[]( void* p, std::size_t size, std::align_val_t align ) noexcept;
