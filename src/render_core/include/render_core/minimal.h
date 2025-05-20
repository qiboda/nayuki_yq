#pragma once

// Vulkan
#ifdef _WIN32                         // 考虑平台是Windows的情况（请自行解决其他平台上的差异）
#    define VK_USE_PLATFORM_WIN32_KHR // 在包含vulkan.h前定义该宏，会一并包含vulkan_win32.h和windows.h
#    define NOMINMAX                  // 定义该宏可避免windows.h中的min和max两个宏与标准库中的函数名冲突
// #pragma comment(lib, "vulkan-1.lib") //链接编译所需的静态存根库
#endif
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_shared.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>

#define GLFW_INCLUDE_VULKAN // 让GLFW使用Vulkan的头文件
#include <GLFW/glfw3.h>

#ifndef RENDER_TYPE_WRAPPER
#    define RENDER_TYPE_WRAPPER( Type )                                                                                \
        using Shared##Type = std::shared_ptr<Type>;                                                                    \
        using Weak##Type = std::weak_ptr<Type>;                                                                        \
        using Unique##Type = std::unique_ptr<Type>;
#endif // RENDER_TYPE_WRAPPER