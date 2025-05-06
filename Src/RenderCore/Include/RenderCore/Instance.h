#pragma once

#include "Core/Misc/NonCopyable.h"
#include <vulkan/vulkan.hpp>

class RenderInstance final : public NonCopyable {

  protected:
    vk::UniqueInstance instance;
};