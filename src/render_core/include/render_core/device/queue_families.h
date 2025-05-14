#pragma once

#include <core/minimal.h>
#include <render_core/minimal.h>

class RENDER_CORE_API QueueFamilies : public IRAII
{
  public:
    QueueFamilies();
    virtual ~QueueFamilies() override;

    virtual void Initialize() override
    {
    }

    virtual void CleanUp() override
    {
    }
};