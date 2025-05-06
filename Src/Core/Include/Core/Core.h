#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <utility>
#include <type_traits>
#include <string>
#include <string_view>

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

CORE_API NY_LOG_CATEGORY_DECLARED(LogCore)
