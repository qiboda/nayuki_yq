module;

// GLM
//  GLM本为是OpenGL设计的，在OpenGL中，NDC（标准化设备坐标系）的深度范围为[-1,
//  1]，而Vulkan中这个范围为[0, 1]，
//  因此我们必须用宏GLM_FORCE_DEPTH_ZERO_TO_ONE来指定深度范围，这样才能获得正确的投影矩阵。
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
// 如果你惯用左手坐标系，在此定义GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module glm;

export using Vec2 = glm::vec2;
export using Vec3 = glm::vec3;
export using Vec4 = glm::vec4;
export using IVec2 = glm::ivec2;
export using IVec3 = glm::ivec3;
export using IVec4 = glm::ivec4;
export using Mat2 = glm::mat2;
export using Mat3 = glm::mat3;
export using Mat4 = glm::mat4;
export using Quat = glm::quat;

export using ::glm::normalize;

// export using glm::dvec2;
// export using glm::dvec3;
// export using glm::dvec4;
// export using glm::dmat2;
// export using glm::dmat3;
// export using glm::dmat4;
