#pragma once

import core;
#include <render_core/render_core.h>

class RENDER_CORE_API QueueFamilies : public IRAII
{
  public:
    QueueFamilies()
    {
    }

    virtual ~QueueFamilies() override
    {
    }

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }
};