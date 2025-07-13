module;

#include "module_export.h"

export module render_core.render_window;

import std;

export class RENDER_CORE_API IRenderWindow
{
  public:
    virtual ~IRenderWindow() = default;

    virtual std::vector<const char*> GetRenderInstanceExtensions() = 0;
};