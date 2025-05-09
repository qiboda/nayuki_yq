#pragma once

#include <vector>      // IWYU pragma: keep
#include <algorithm>   // IWYU pragma: keep
#include <memory>      // IWYU pragma: keep
#include <utility>     // IWYU pragma: keep
#include <type_traits> // IWYU pragma: keep
#include <string>      // IWYU pragma: keep
#include <string_view> // IWYU pragma: keep
#include <functional>  // IWYU pragma: keep
// 可以使用 not, or, and 等关键字
#include <ciso646> // IWYU pragma: keep

// GLM
//  GLM本为是OpenGL设计的，在OpenGL中，NDC（标准化设备坐标系）的深度范围为[-1,
//  1]，而Vulkan中这个范围为[0, 1]，
//  因此我们必须用宏GLM_FORCE_DEPTH_ZERO_TO_ONE来指定深度范围，这样才能获得正确的投影矩阵。
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
// 如果你惯用左手坐标系，在此定义GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ModuleExport.h>

#include <Core/TemplateUtility.h>
#include <Core/Macro/Macro.h>
#include <Core/TypeDef.h>
#include <Core/Misc/Singleton.h>
#include <Core/Logger/Logger.h>

CORE_API NY_LOG_CATEGORY_DECLARED( LogCore )
