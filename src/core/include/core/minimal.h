/// 这个头文件是Core模块的最小化头文件，
/// 只包含了Core模块中没有cpp的一部分头文件，和外部库的核心头文件，
#pragma once

#include <vector>        // IWYU pragma: keep
#include <algorithm>     // IWYU pragma: keep
#include <memory>        // IWYU pragma: keep
#include <utility>       // IWYU pragma: keep
#include <type_traits>   // IWYU pragma: keep
#include <string>        // IWYU pragma: keep
#include <string_view>   // IWYU pragma: keep
#include <functional>    // IWYU pragma: keep
#include <concepts>      // IWYU pragma: keep
#include <map>           // IWYU pragma: keep
#include <unordered_map> // IWYU pragma: keep
#include <set>           // IWYU pragma: keep
#include <unordered_set> // IWYU pragma: keep
#include <atomic>        // IWYU pragma: keep
#include <format>        // IWYU pragma: keep
#include <thread>        // IWYU pragma: keep
#include <mutex>         // IWYU pragma: keep
#include <atomic>        // IWYU pragma: keep
#include <optional>      // IWYU pragma: keep

// GLM
//  GLM本为是OpenGL设计的，在OpenGL中，NDC（标准化设备坐标系）的深度范围为[-1,
//  1]，而Vulkan中这个范围为[0, 1]，
//  因此我们必须用宏GLM_FORCE_DEPTH_ZERO_TO_ONE来指定深度范围，这样才能获得正确的投影矩阵。
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
// 如果你惯用左手坐标系，在此定义GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <module_export.h>

#include <core/template_utility.h>
#include <core/macro/macro.h>
#include <core/type_def.h>
#include <core/misc/singleton.h>
#include <core/misc/non_copyable.h>
#include <core/misc/raii.h>
#include <core/misc/tickable.h>
#include <core/compiler/diagnostic.h>
#include <core/compiler/optimize.h>
#include <core/logger/log.h>
#include <core/logger/category.h>
#include <core/concepts/derived.h>