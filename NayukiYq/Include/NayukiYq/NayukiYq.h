#pragma once

#ifndef NAYUKIYQ_H
#define NAYUKIYQ_H

// This library need to set NayukiYq_EXPORTS macro to export API for external
// usage.
#ifdef NAYUKIYQ_EXPORTS
#define NAYUKIYQ_API __declspec(dllexport)
#else
#define NAYUKIYQ_API __declspec(dllimport)
#endif

// std
#include <cassert>

#include <utility>

#include <memory>

#include <algorithm>

#include <array>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <fstream>
#include <sstream>
#include <string>

#include <cwchar>
#include <iostream>

// GLM
//  GLM本为是OpenGL设计的，在OpenGL中，NDC（标准化设备坐标系）的深度范围为[-1,
//  1]，而Vulkan中这个范围为[0, 1]，
//  因此我们必须用宏GLM_FORCE_DEPTH_ZERO_TO_ONE来指定深度范围，这样才能获得正确的投影矩阵。
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
// 如果你惯用左手坐标系，在此定义GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Vulkan
#ifdef _WIN32 // 考虑平台是Windows的情况（请自行解决其他平台上的差异）
#define VK_USE_PLATFORM_WIN32_KHR // 在包含vulkan.h前定义该宏，会一并包含vulkan_win32.h和windows.h
#define NOMINMAX // 定义该宏可避免windows.h中的min和max两个宏与标准库中的函数名冲突
// #pragma comment(lib, "vulkan-1.lib") //链接编译所需的静态存根库
#endif
#include <vulkan/vulkan.hpp>

// self
#include <NayukiYq/TypeDef.h>
#include <NayukiYq/Logger/Logger.h>
// #include <NayukiYq/Common/Macro.h>

#endif // !NAYUKIYQ_H