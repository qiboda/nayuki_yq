module;

#include "module_export.h"
#include <render_core/render_core.h>

export module render_core.device.queue_families;

import core;
import core.misc.iraii;

export class RENDER_CORE_API QueueFamilies : public IRAII
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